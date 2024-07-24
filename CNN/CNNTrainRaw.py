import os
import numpy as np # type: ignore
import tensorflow as tf # type: ignore
from tensorflow.keras.models import load_model, save_model # type: ignore
from tensorflow.keras.preprocessing.sequence import pad_sequences # type: ignore
from tensorflow.keras import * # type: ignore
from tensorflow.keras.layers import * # type: ignore
from nnom import * # type: ignore
import matplotlib.pyplot as plt # type: ignore
import re

# 动作分类名
motion_names = ['RightAngle','SharpAngle','Lightning','Triangle','Letter_h','letter_R','letter_W','letter_phi','Circle','UpAndDown','Horn','Wave','NoMotion']

# 定义目录路径
DEF_SAVE_TO_PATH = './TraningData_7_23/'
DEF_MODEL_NAME = 'model.h5'
DEF_MODEL_H_NAME = 'weights.h'
DEF_FILE_MAX = 100
DEF_N_ROWS = 60

# 文件格式
DEF_FILE_FORMAT = '.txt'
# 文件名分隔符
DEF_FILE_NAME_SEPERATOR = '_'
DEF_BATCH_SIZE = 80
DEF_NUM_EPOCH = 160

# 检查TensorFlow是否识别到了GPU
print("Num GPUs Available: ", len(tf.config.list_physical_devices('GPU')))

# 动作名称到标签的映射
motion_to_label = {name: idx for idx, name in enumerate(motion_names)}

def train(x_train, x_test,input_shape=(DEF_N_ROWS, 3), num_classes=len(motion_names),batch_size= DEF_BATCH_SIZE, epochs = DEF_NUM_EPOCH):
    inputs = Input(shape=input_shape) # type: ignore
    # 卷积层1
    x = Conv1D(24, kernel_size=3,strides=1)(inputs)  # type: ignore # 减少滤波器数量
    x = ReLU()(x)  # type: ignore 
    x = Conv1D(12, kernel_size=3,strides=1)(x)  # type: ignore # 减少滤波器数量
    x = ReLU()(x)  # type: ignore
    x = Conv1D(12, kernel_size=3,strides=1)(x)  # type: ignore # 减少滤波器数量
    x = ReLU()(x)  # type: ignore
    x = Conv1D(12, kernel_size=3,strides=1)(x)  # type: ignore # 减少滤波器数量
    x = ReLU()(x)  # type: ignore
    x = MaxPooling1D(pool_size=3, strides=1)(x) # type: ignore
    # 展平层
    x = Flatten()(x) # type: ignore
    x = Dropout(0.5)(x) # type: ignore
    # 全连接层1
    x = Dense(num_classes)(x) # type: ignore
    x = Dropout(0.5)(x)  # type: ignore # 添加Dropout层
    outputs = Softmax()(x) # type: ignore
    
    model = Model(inputs=inputs, outputs=outputs) # type: ignore
    
    # 编译模型
    model.compile(optimizer='adam',
              loss='CategoricalCrossentropy',
              metrics=['accuracy'])
    
    model.summary()
    
    # save best
    history = model.fit(x_train,
              batch_size=batch_size,
              epochs=epochs,
              verbose=2,
              validation_data=(x_test),
              shuffle=True)
    
    save_model(model, DEF_MODEL_NAME)
    del model
    tf.keras.backend.clear_session()
    return history


# 加载数据集
def load_dataset(root_dir, nrows=None):
    file_list = []
    labels = []
    for filename in os.listdir(root_dir):
        if filename.endswith(DEF_FILE_FORMAT):
            match = re.match(rf'^([\w]+)_([\d]+){DEF_FILE_FORMAT}$', filename)
            if match:
                motion_name = match.group(1)
                number_str = match.group(2)
                number = int(number_str)
                if 0 <= number <= DEF_FILE_MAX:
                    if motion_name in motion_to_label:
                        file_path = os.path.join(root_dir, filename)
                        # 使用nrows参数限制读取的行数
                        data = np.loadtxt(file_path, delimiter=' ', usecols=(0, 1, 2), max_rows=nrows)
                        file_list.append(data)
                        labels.append(motion_to_label[motion_name])
                    else:
                        print(f"Motion name not recognized: {filename}")
                else:
                    print(f"Number out of range: {filename}")
            else:
                print(f"Invalid file name format: {filename}")
    return file_list, labels

file_list, labels = load_dataset(DEF_SAVE_TO_PATH,DEF_N_ROWS)

# 数据预处理，例如填充序列以达到统一长度
max_len = max([len(x) for x in file_list])  # 找到最长序列的长度
print(f"Max length of sequences: {max_len}")  # 打印max_len的值
file_list_padded = pad_sequences(file_list, maxlen=max_len, dtype='float32', padding='post', value=0)
    
# 转换标签为one-hot编码
labels_one_hot = tf.keras.utils.to_categorical(labels, num_classes=len(motion_names))

# 转换为TensorFlow数据集
dataset = tf.data.Dataset.from_tensor_slices((file_list_padded, labels_one_hot))  # 使用one-hot编码的标签

# 计算分割点前，先确保数据集被完全构建
num_elements = dataset.reduce(0, lambda x, _: x + 1).numpy()

train_size = int(num_elements * 0.8)

# 先shuffle再分割
dataset = dataset.shuffle(buffer_size=10000)  # 调整buffer_size以适合你的数据集大小

# 分割数据集
train_dataset = dataset.take(train_size)
test_dataset = dataset.skip(train_size)

# 批处理数据集
batch_size = DEF_BATCH_SIZE
train_dataset = train_dataset.batch(batch_size)
test_dataset = test_dataset.batch(batch_size)

#训练模型
history = train(train_dataset,test_dataset, batch_size=DEF_BATCH_SIZE, epochs=DEF_NUM_EPOCH)

#加载模型
model = load_model(DEF_MODEL_NAME)

model.compile(optimizer='adam',
            loss='CategoricalCrossentropy',
            metrics=['accuracy'])

model.summary()

# 从训练数据集中获取一个批次作为校准数据集
x_test, _ = next(iter(test_dataset))
# 将EagerTensor转换为NumPy数组
x_test = x_test.numpy()

# (NNoM utils) Automatically deploying, use 100 pices for output range
generate_model(model, x_test, format='hwc', name=DEF_MODEL_H_NAME) # type: ignore
