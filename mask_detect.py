from PyQt5 import QtWidgets, QtGui, QtCore
import mysql.connector
import sys
import torch
import subprocess
import threading
import os
import glob
import cv2
import numpy as np
import csv
import re

MAX_WIDTH = 600
MAX_HEIGHT = 500


class MainWindow(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()

        # 加载模型
        self.model = torch.hub.load('ultralytics/yolov5', 'custom', path='../yolov5/runs/train/exp1/weights/best.pt')

        # 摄像头控制 处理后视频全局地址
        self.cap = None
        self.video_path = None

        self.setWindowTitle("Mask detection")

        '''
        * 布局和结构 ： 四个控件 两个定时器 + csv定时器
        * 画布 ：            相机画面 和 视频
                               |
        * 相机按钮： 摄像头控制 模型处理 数据库或者csv文件

        * 视频导入： 导入视频模型处理 数据库或者csv文件
        * 视频播放： 播放视频 暂停视频

           |         画布         |
           |     相机控制按钮       |
           |视频导入按钮  视频播放按钮|
        '''

        # label作为画布展示视频和相机画面
        self.label = QtWidgets.QLabel()
        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(self.label)

        self.button = QtWidgets.QPushButton("Toggle Camera")
        self.button.clicked.connect(self.toggle_camera)
        layout.addWidget(self.button)

        # 创建两个新的按钮并添加到一个新的水平布局中
        self.button_layout = QtWidgets.QHBoxLayout()

        self.load_video_button = QtWidgets.QPushButton("Load Video")
        self.load_video_button.clicked.connect(self.load_video)

        self.play_pause_button = QtWidgets.QPushButton("Play/Pause")
        self.play_pause_button.clicked.connect(self.play_pause_video)

        self.button_layout.addWidget(self.load_video_button)
        self.button_layout.addWidget(self.play_pause_button)
        layout.addLayout(self.button_layout)

        self.csv_checkbox = QtWidgets.QCheckBox("CSV")
        self.csv_checkbox.stateChanged.connect(self.csv_timer_controller)
        layout.insertWidget(0, self.csv_checkbox)

        self.setLayout(layout)

        self.resize(MAX_WIDTH, MAX_HEIGHT)

        # 相机和视频数据库链接信号
        '''
        相机数据库：成功链接打开两个定时器，否则只打开毫秒定时器
        视频数据库：成功链接播放视频的时候传输数据到数据库，否则不传输

        '''
        self.cmr_db_cnt_signal = False
        self.vd_db_cnt_signal = False

        ''' 
        双定时器 
        timer： 控制相机画面处理（1ms）
        record_timer： 控制数据库记录（5s） 
        '''
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.update_frame)

        self.record_timer = QtCore.QTimer()
        self.record_timer.timeout.connect(self.insert_mask_count)
        self.cmr_db_time_count = 1

        self.csv_timer = QtCore.QTimer()
        self.csv_timer.timeout.connect(self.cmr_csv_record)
        self.cmr_csv_time_count = 1

    def __del__(self):
        self.cursor.close()
        self.cnx.close()

    ''' 数据库链接和载入数据函数 '''

    def camera_connect_to_database(self):
        try:
            self.cnx = mysql.connector.connect(user='root', password='123456',
                                               host='localhost',
                                               database='camera_for_test')
            self.cursor = self.cnx.cursor()
            self.cmr_db_cnt_signal = True
            print("Database connected successfully!")

        except mysql.connector.Error as err:
            print(f"Something went wrong: {err}")

    def video_connect_to_database(self):
        try:
            self.cnx = mysql.connector.connect(user='root', password='123456',
                                               host='localhost',
                                               database='video_for_test')
            self.cursor = self.cnx.cursor()
            self.vd_db_cnt_signal = True
            print("Database connected successfully!")

        except mysql.connector.Error as err:
            print(f"Something went wrong: {err}")

    ''' 
    数据库的事务提交！！！
    双端正常 但是数据无法传输
    '''

    def insert_mask_count(self):

        if self.cap is None or not self.cap.isOpened():
            return

        # 读取当前帧
        _, frame = self.cap.read()

        # 使用模型进行预测
        results = self.model(frame)

        # 计算 mask 和 no_mask 的人数
        mask_count = sum(1 for *_, cls in results.xyxy[0] if results.names[int(cls)] == 'mask')
        no_mask_count = sum(1 for *_, cls in results.xyxy[0] if results.names[int(cls)] == 'no_mask')

        # 将这些信息存储到数据库中
        try:
            create_table_query = """
            CREATE TABLE IF NOT EXISTS mask_count (
                id INT AUTO_INCREMENT,
                time INT,
                mask INT,
                no_mask INT,
                PRIMARY KEY(id)
            );
            """
            self.cursor.execute(create_table_query)
            self.cursor.execute('''INSERT INTO mask_count(time, mask, no_mask)
                                   VALUES (%s, %s, %s)''', (self.cmr_db_time_count * 5, mask_count, no_mask_count))
        except mysql.connector.Error as err:
            print(f"Something went wrong: {err}")

        self.cnx.commit()
        self.cmr_db_time_count += 1

    '''
    相机或者视频模型处理后的数量写入CSV文件
    '''

    def csv_timer_controller(self):
        if self.csv_checkbox.isChecked():
            self.csv_timer.start(5000)
        else:
            self.csv_timer.stop()

    def cmr_csv_record(self):
        if self.cap is None or not self.cap.isOpened():
            return

        # 读取当前帧
        _, frame = self.cap.read()

        # 使用模型进行预测
        results = self.model(frame)

        # 计算 mask 和 no_mask 的人数
        mask_count = sum(1 for *_, cls in results.xyxy[0] if results.names[int(cls)] == 'mask')
        no_mask_count = sum(1 for *_, cls in results.xyxy[0] if results.names[int(cls)] == 'no_mask')

        file_path = './csv/camera_record.csv'
        if self.cmr_db_time_count == 1:
            if os.path.exists(file_path):
                os.remove(file_path)
            with open(file_path, 'w') as f:
                f.write('Time,Mask Count,No Mask Count\n')

        with open(file_path, 'a') as f:
            f.write(f'{self.cmr_db_time_count * 5},{mask_count},{no_mask_count}\n')
        self.cmr_csv_time_count += 1

    ''' 相机控制 和 画面处理函数（time中断服务函数）'''

    def update_frame(self):
        if self.cap is None:
            return

        ret, frame = self.cap.read()
        if not ret:
            return

        # 使用模型进行预测
        results = self.model(frame)

        # 在帧上绘制预测结果
        for *box, conf, cls in results.xyxy[0]:
            label = f'{results.names[int(cls)]}: {conf:.2f}'
            frame = cv2.rectangle(frame, (int(box[0]), int(box[1])), (int(box[2]), int(box[3])), (255, 0, 0), 2)
            frame = cv2.putText(frame, label, (int(box[0]), int(box[1]) - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9,
                                (255, 0, 0), 2)

        # 转为为 PyQt 图像格式
        qtImg = QtGui.QImage(frame.data, frame.shape[1], frame.shape[0], QtGui.QImage.Format_RGB888).rgbSwapped()
        self.label.setPixmap(QtGui.QPixmap.fromImage(qtImg))  # 加载 PyQt 图像

    def toggle_camera(self):
        # 打开相机
        if self.cap is None:
            self.cap = cv2.VideoCapture(0)
            self.camera_connect_to_database()
            self.timer.start(1)
            if self.cmr_db_cnt_signal is True:
                self.record_timer.start(5000)
        # 关闭相机
        elif self.cap.isOpened():
            self.cap.release()
            self.cap = None
            self.cmr_db_time_count = 0
            self.record_timer.stop()
            self.timer.stop()
            self.cnx.close()
        else:
            self.cap = cv2.VideoCapture(0)
            self.record_timer.start(5000)
            self.timer.start(1)

    ''' 视频载入和处理 视频播放和暂停'''

    def load_video(self):
        video_path, _ = QtWidgets.QFileDialog.getOpenFileName(self, "Open Video File", "",
                                                              "Video Files (*.avi *.mp4 *.flv *.mkv)")
        if video_path:
            output_directory = './videoStore'
            model_path = '../yolov5/runs/train/exp1/weights/best.pt'
            detect_path = '../yolov5/detect.py'
            command = f'python {detect_path} --save-txt --weights {model_path} --source {video_path} --project {output_directory}'
            process = subprocess.Popen(command, shell=True)

            process.wait()

            exp_dirs = glob.glob(f'{output_directory}/exp*')
            latest_exp_dir = max(exp_dirs, key=os.path.getmtime)

            mp4_files = glob.glob(f'{latest_exp_dir}/*.mp4')
            latest_mp4_file = max(mp4_files, key=os.path.getmtime)
            self.video_path = latest_mp4_file

            process.terminate()

    def play_pause_video(self):
        if self.cap is None:
            if self.video_path is not None:
                self.cap = cv2.VideoCapture(self.video_path)
                # self.video_connect_to_database()
                # new_thread = threading.Thread(target=self.vd_insert_db_csv)
                # # if self.vd_db_cnt_signal is True or self.csv_checkbox.isChecked():
                # new_thread.start()
            else:
                return

        if not self.timer.isActive():
            # 如果视频已经到达结尾，将其重新定位到开始位置
            if self.cap.get(cv2.CAP_PROP_POS_FRAMES) == self.cap.get(cv2.CAP_PROP_FRAME_COUNT):
                self.cap.set(cv2.CAP_PROP_POS_FRAMES, 0)

            self.timer.start(1)
            self.play_pause_button.setText("Pause")
        else:
            self.timer.stop()
            self.play_pause_button.setText("Play")

    # 一个新的线程函数
    # 传输视频每一帧的图像中的mask和no_mask到csv或者数据库
    def vd_insert_db_csv(self):
        cap = cv2.VideoCapture(self.video_path)
        fps = cap.get(cv2.CAP_PROP_FPS)
        cap.release()

        exp_dirs = glob.glob('./videoStore/exp*')

        def extract_number(dir):
            match = re.search(r'\d+$', dir)
            return int(match.group()) if match else 0

        latest_exp_dir = max(exp_dirs, key=extract_number)

        # 构造labels文件夹的路径
        labels_dir = os.path.join(latest_exp_dir, 'labels')

        # 列出labels文件夹下的所有文件
        txt_files = os.listdir(labels_dir)

        def is_number(s):
            return re.fullmatch(r'\d+', s) is not None

        selected_files = [file for file in txt_files if
                          is_number(os.path.splitext(os.path.basename(file))[0].split('_')[-1]) and int(
                              os.path.splitext(os.path.basename(file))[0].split('_')[-1]) % fps == 0]

        selected_files = [os.path.join(labels_dir, file) for file in selected_files]

        def extract_number_from_filename(filename):
            match = re.search(r'_(\d+)', filename)
            return int(match.group(1)) if match else 0

        selected_files = sorted(selected_files, key=extract_number_from_filename)

        results = []
        for file in selected_files:
            data = np.loadtxt(file)
            mask_count = np.sum(data[:, 0] == 0)
            no_mask_count = np.sum(data[:, 0] == 1)
            results.append((mask_count, no_mask_count))

        # 如果勾选了csv， 写入csv文件中
        if self.csv_checkbox.isChecked():
            with open('./csv/vd_csv/vd.csv', 'w', newline='') as file:
                writer = csv.writer(file)
                writer.writerow(["Time", "Mask", "No Mask"])

                for i, (mask_count, no_mask_count) in enumerate(results):
                    writer.writerow([i + 1, mask_count, no_mask_count])

        print(results)
        # 如果链接了数据库，写入数据库中
        if self.vd_db_cnt_signal is True:
            for i, (mask_count, no_mask_count) in enumerate(results):
                try:
                    create_table_query = """
                    CREATE TABLE IF NOT EXISTS mask_count (
                        id INT AUTO_INCREMENT,
                        time INT,
                        mask INT,
                        no_mask INT,
                        PRIMARY KEY(id)
                    );
                    """
                    self.cursor.execute(create_table_query)

                    self.cursor.execute('''INSERT INTO mask_count(time, mask, no_mask)
                                           VALUES (%s, %s, %s)''', (int(i + 1), int(mask_count), int(no_mask_count)))
                except mysql.connector.Error as err:
                    print(f"Something went wrong: {err}")

            self.cnx.commit()
            self.cursor.close()
            self.cnx.close()


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    MainWin = MainWindow()
    MainWin.show()
    sys.exit(app.exec_())
