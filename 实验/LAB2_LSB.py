from PIL import Image

# 字符串转二进制
def str_to_binary(str):
    binary_string = ''
    for c in str:
        ascii_value = ord(c)
        binary_string += format(ascii_value, '08b')
    return binary_string

# 二进制转字符串
def binary_to_str(binary_string):
    str = ''
    for i in range(0, len(binary_string), 8):
        byte_string = binary_string[i:i+8]
        ascii_value = int(byte_string, 2)
        str += chr(ascii_value)
    return str

# 信息隐藏
def hide_message(image_path, message):
    image = Image.open(image_path)  # 读入原始图片
    binary_message = str_to_binary(message)  # 将隐藏信息转化为二进制
    binary_Message = format(len(binary_message), '032b') + binary_message  # 在隐藏信息之前加上32位信息隐藏量
    key = len(binary_Message)
    encoded_image = image  # 信息隐藏后的图片
    # 确定大小
    width, height = image.size
    total_pixel = width * height  # 总像素数
    
    if key > total_pixel * 3:
        raise ValueError("Key is larger than total pixels in the image.")
    
    message_processed = 0  # 已经加密的信息量
    for x in range(width):
        for y in range(height):
            if message_processed < key:
                pixel = list(image.getpixel((x, y)))    # RGB的3个8bit像素
                for i in range(len(pixel)):
                    if message_processed < key:
                        int_binary_message = int(binary_Message[message_processed], 2)  # 隐藏信息比特位
                        pixel[i] = pixel[i] & 0b11111110            # 将像素的最低位 置0
                        pixel[i] = pixel[i] | int_binary_message    # 将隐藏信息写入像素的最低位
                        message_processed += 1
                    encoded_image.putpixel((x, y), tuple(pixel))  # 将(x,y)位置的像素值更新:[r,g,b]=tuple(pixel)
    return encoded_image

# 提取信息
def extract_message(encoded_image_path):
    encoded_image = Image.open(encoded_image_path)  # 读入信息隐藏图片
    # 确定大小
    width, height = encoded_image.size
    total_pixels = width * height

    key_processed = 0
    binary_key = ""  # 信息量二进制串
    for x in range(width):
        for y in range(height):
            pixel = list(encoded_image.getpixel((x, y)))
            for i in range(len(pixel)):
                if key_processed < 32:
                    extracted_bit = pixel[i] & 0b00000001         # 提取像素的最低1bit
                    binary_key += str(extracted_bit)
                key_processed += 1
    key = int(binary_key, 2)  # 将二进制信息量转化为整数

    if key + 32 > total_pixels * 3:  # 判断是否包含了足够的像素来提取出所有的信息
        raise ValueError("Key and metadata are larger than total pixels in the image.")

    message_processed = 0
    binary_message = ""
    for x in range(width):
        for y in range(height):
            pixel = list(encoded_image.getpixel((x, y)))
            for i in range(len(pixel)):
                message_processed += 1
                if (message_processed > 32) and (message_processed <= key + 32):  # 否则读取字符串过长
                    extracted_bit = pixel[i] & 0b00000001         # 提取像素的最低1bit
                    binary_message += str(extracted_bit)    # 将提取的bit添加到二进制消息中
    extracted_message = binary_to_str(binary_message)
    return extracted_message

if __name__ == "__main__":
    flag = True
    while flag:
        print("*****LSB算法加密/解密*****")
        choice = input("请选择：A、加密  B、解密  C、退出\n")
        if choice == 'A':
            # 读入图像（建议png，否则会被计算机压缩而失去隐藏信息）
            image_path = input("请输入需要加密图片的路径：\n")  # C:/Users/lenovo/Desktop/CQUlibrary.png
            message = input("请输入需要隐藏的信息：\n")  # CQUWATERMASKEXP
            encoded_image = hide_message(image_path, message)
            encoded_image_path = input("请输入加密图片保存路径：\n")  # C:/Users/lenovo/Desktop/CQUlibrary_hidden.png
            encoded_image.save(encoded_image_path)
            print("信息隐藏成功！\n")
        
        if choice == 'B':
            encoded_image_path = input("请输入需要解密图片的路径：\n")
            extracted_message = extract_message(encoded_image_path)
            print("提取出来的隐藏信息:", extracted_message)
            print("信息提取成功！\n")

        if choice == 'C':
            flag = False
