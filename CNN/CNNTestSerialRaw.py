import serial # type: ignore
import serial.tools.list_ports # type: ignore
import numpy as np # type: ignore
import tensorflow as tf # type: ignore
from tensorflow.keras.models import load_model # type: ignore
DEF_TITLE_STRING = 'IMU\n'
DEF_MODEL_NAME = 'model.h5'
DEF_BAUD_RATE = 921600
DEF_N_ROWS = 60

motion_name = ['RightAngle','SharpAngle','Lightning','Triangle','Letter_h','letter_R','letter_W','letter_phi','Circle','UpAndDown','Horn','Wave','NoMotion']

port_list = list(serial.tools.list_ports.comports())

#显示所有可用串口
#Show all the port avaliable on current device
def show_all_com():
    index = 0
    port_list_name = []
    if len(port_list) <= 0:
        print("the serial port can't find!")
    else:
        for itms in port_list:
            index += 1
            print(index,':',list(itms)[0], list(itms)[1])
            port_list_name.append(itms.device)

    return port_list_name

#请求用户根据索引值选择并初始化一个串口
#Ask a index number for serial port from user and initializes it.
def Serial_init():
    port_avaliable = show_all_com()
    port_avaliable_size = len(port_avaliable)
    index = int(input("\nChoose a port:"))
    
    #
    while index > port_avaliable_size or index <= 0:
        if  index > port_avaliable_size or index <= 0:
            print("Invalid Input.Check and try again")
            index = int(input("Choose a port:"))
            
    print(f"\nSerial port",port_avaliable[index - 1],f"initalized successfully.\nBaud rate:{DEF_BAUD_RATE} Byte size:8  Parity:None  Stop bit:1")
    
    return serial.Serial(port=port_avaliable[index - 1],
                                baudrate=DEF_BAUD_RATE,
                                bytesize=serial.EIGHTBITS,
                                parity=serial.PARITY_NONE,
                                stopbits=serial.STOPBITS_ONE,
                                timeout=0.5) 
    
#检查数据串口数据标题是否符合定义的标题，数据的标题可以修改DEF_TITLE_STRING
#Check if the title from recerived data same as DEF_TITLE_STRING.
def Check_Title(Received_String):
    i = 0
    Title_Buffer = ''
    while Received_String[i] != '\n':
        Title_Buffer += Received_String[i]
        i += 1
    Title_Buffer+='\n'
    if(Title_Buffer == DEF_TITLE_STRING):
        return True
    else:
        return False
    
#从数据标题之后裁剪出可用的数据
#Cut off the tiltle from recerived data.
def IMU_String(Received_String):
        return Received_String[len(DEF_TITLE_STRING)::1]
    
def Str2Array(str):
    # 将字符串分割成行
    lines = str.split('\n')

    # 使用列表推导式将每一行转换为列表，并取第1列、第2列和第3列
    data_list = [list(map(float, [line.split()[0], line.split()[1], line.split()[2]])) for line in lines if line.strip()]
    
    # 将列表转换为NumPy数组
    data_array = np.array(data_list)
    data_array = np.array(data_list).T  # 转置数组
    
    return data_array

# 定义 num_classes
num_classes = len(motion_name)

model = load_model(DEF_MODEL_NAME)

def predict_motion(inputs_array):
    # 转换为张量并增加批次维度
    inputs_tensor = tf.convert_to_tensor(inputs_array, dtype=tf.float32)
    inputs_tensor = tf.expand_dims(inputs_array, axis=0)  # 现在形状为 (1, channels, time_steps)
    
    # 现在可以安全地访问inputs
    outputs = model(inputs_tensor)
    print(outputs)
    index = 0
    temp = 0
    for i in range(0,12,1):
        if outputs[0][i] > temp:
            temp = outputs[0][i]
            index = i
    
    print(temp)
    if temp >= 0.90:
        return motion_name[index]
    else:
        return "Unrecognized"

# 主函数
def main():
    serial_using = Serial_init()
    user_input = 'y'
    while user_input == 'y':
        test_count = int(input("How many tests:"))
        while test_count >= 0:
            received = serial_using.readall().decode("ASCII")
            if len(received) != 0 and Check_Title(received):
                test_string = IMU_String(received)
                inputs_array = Str2Array(test_string)
                inputs_array = inputs_array.T
                print(predict_motion(inputs_array))
                test_count -= 1
        user_input = input("Again? (y/n)")
    print("Bye!")

main()
