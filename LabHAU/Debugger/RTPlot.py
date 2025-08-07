import serial
import struct
import time
import collections
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# --- CẤU HÌNH ---
# Chế độ mô phỏng: True để dùng dữ liệu giả, False để đọc từ cổng COM thật
SIMULATION_MODE = False 

SERIAL_PORT = 'COM5'  # Thay đổi thành cổng COM của bạn (ví dụ: 'COM3' trên Windows, '/dev/ttyUSB0' trên Linux)
BAUD_RATE = 6000000

# Cấu trúc khung truyền
FRAME_START_BYTE = 0x03
FRAME_END_BYTE = 0xFC
FRAME_LENGTH = 22  # 1 start + 5 * 4 bytes data + 1 end
NUM_DATA_POINTS = 5

# Cấu hình đồ thị
PLOT_WINDOW_SIZE = 200  # Số điểm dữ liệu hiển thị trên đồ thị
PLOT_UPDATE_INTERVAL = 5  # Thời gian cập nhật đồ thị (ms)
# --- KẾT THÚC CẤU HÌNH ---

# Khởi tạo bộ đệm (deque) để lưu trữ dữ liệu cho 5 kênh
data_channels = [collections.deque(np.zeros(PLOT_WINDOW_SIZE), maxlen=PLOT_WINDOW_SIZE) for _ in range(NUM_DATA_POINTS)]

# Khởi tạo đối tượng serial
ser = None
if not SIMULATION_MODE:
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Đã kết nối thành công đến cổng {SERIAL_PORT} với baudrate {BAUD_RATE}")
    except serial.SerialException as e:
        print(f"Lỗi: Không thể mở cổng {SERIAL_PORT}. {e}")
        print("Chương trình sẽ thoát.")
        exit()

def read_and_parse_data():
    """
    Hàm đọc và phân tích một khung dữ liệu từ cổng COM.
    """
    if not ser:
        return None

    while True:
        start_byte = ser.read(1)
        if not start_byte:
            print("Chờ dữ liệu...")
            return None
        if start_byte[0] == FRAME_START_BYTE:
            break
            
    frame_data = ser.read(FRAME_LENGTH - 1)
    
    if len(frame_data) < (FRAME_LENGTH - 1):
        print("Khung dữ liệu không đủ độ dài.")
        return None

    if frame_data[-1] != FRAME_END_BYTE:
        print("Lỗi byte kết thúc.")
        return None

    try:
        unpacked_data = struct.unpack('<5i', frame_data[0:20])
        return list(unpacked_data)
    except struct.error as e:
        print(f"Lỗi giải nén dữ liệu: {e}")
        return None

def generate_simulation_data(time_step):
    """
    Hàm tạo dữ liệu giả để mô phỏng.
    """
    d1 = 100 * np.sin(2 * np.pi * 0.1 * time_step) + np.random.randn() * 2
    d2 = 75 * np.cos(2 * np.pi * 0.25 * time_step) + 50
    d3 = 50 * np.sin(2 * np.pi * 0.5 * time_step) + np.random.randn() * 5
    d4 = 150 * (time_step % 50) / 50.0 
    d5 = np.random.randint(-100, 100) 
    return [d1, d2, d3, d4, d5]

# --- THAY ĐỔI Ở ĐÂY ---
# Thiết lập đồ thị
fig, ax = plt.subplots(figsize=(12, 7))

# Danh sách tên cho từng đường dữ liệu
data_labels = ['McUs', 'McIs', 'McIu', 'McIv', 'McIw']

# Tạo các đường đồ thị với tên tương ứng
lines = [ax.plot([], [], lw=2, label=data_labels[i])[0] for i in range(NUM_DATA_POINTS)]

ax.set_title('Đồ thị dữ liệu thời gian thực từ cổng COM')
ax.set_xlabel('Mẫu')
ax.set_ylabel('Giá trị')
ax.legend() # Hiển thị tên các đường (McUs, McIs,...)
ax.grid(True)
ax.set_xlim(0, PLOT_WINDOW_SIZE)
ax.set_ylim(-350000, 350000) 
# --- KẾT THÚC THAY ĐỔI ---

simulation_time_step = 0

def update_plot(frame):
    """
    Hàm cập nhật đồ thị, được gọi lặp đi lặp lại bởi FuncAnimation.
    """
    global simulation_time_step
    
    new_data = None
    if SIMULATION_MODE:
        new_data = generate_simulation_data(simulation_time_step)
        simulation_time_step += 0.1
    else:
        new_data = read_and_parse_data()

    if new_data and len(new_data) == NUM_DATA_POINTS:
        for i in range(NUM_DATA_POINTS):
            data_channels[i].append(new_data[i])
            lines[i].set_data(range(PLOT_WINDOW_SIZE), data_channels[i])
        
        ax.relim()
        ax.autoscale_view(scalex=False, scaley=True)
            
    return lines

# Tạo animation
ani = FuncAnimation(
    fig, 
    update_plot, 
    interval=PLOT_UPDATE_INTERVAL, 
    blit=True,
    cache_frame_data=False
)

# Hiển thị đồ thị
plt.show()

# Đóng cổng COM khi cửa sổ đồ thị được đóng
if ser and ser.is_open:
    ser.close()
    print(f"Đã đóng cổng {SERIAL_PORT}.")