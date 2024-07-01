import rsa
from PIL import Image

# 字节转化为二进制字符串
def bytes_to_binary_string(data):
    return "".join(format(byte, '08b') for byte in data)

# 二进制字符串转化为字节
def binary_string_to_bytes(binary_string):
    # 检查二进制字符串的长度是否为8的倍数
    if len(binary_string) % 8 != 0:
        raise ValueError("Binary string length must be a multiple of 8")
    # 将二进制字符串转换为字节对象
    return bytes(int(binary_string[i:i+8], 2) for i in range(0, len(binary_string), 8))

# 信息隐藏
def hide_message(image_path, encrypted_data, signature):
# def hide_message(image_path, message):
    image = Image.open(image_path)  # 读入原始图片
    binary_encrypted_data = bytes_to_binary_string(encrypted_data)  # 加密信息
    binary_signature = bytes_to_binary_string(signature)  # 签名
    binary_message = format(len(binary_encrypted_data), '032b') + binary_encrypted_data + format(len(binary_signature), '032b') + binary_signature
    key = len(binary_message)
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
                        int_binary_message = int(binary_message[message_processed], 2)  # 隐藏信息比特位
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

    key1_processed = 0
    binary_key1 = ""  # 信息量二进制串
    for x in range(width):
        for y in range(height):
            pixel = list(encoded_image.getpixel((x, y)))
            for i in range(len(pixel)):
                if key1_processed < 32:
                    extracted_bit = pixel[i] & 0b00000001         # 提取像素的最低1bit
                    binary_key1 += str(extracted_bit)
                key1_processed += 1
    key1 = int(binary_key1, 2)  # 将二进制信息量转化为整数

    message1_processed = 0
    binary_message1 = ""
    for x in range(width):
        for y in range(height):
            pixel = list(encoded_image.getpixel((x, y)))
            for i in range(len(pixel)):
                message1_processed += 1
                if (message1_processed > 32) and (message1_processed <= key1 + 32):  # 否则读取字符串过长
                    extracted_bit = pixel[i] & 0b00000001         # 提取像素的最低1bit
                    binary_message1 += str(extracted_bit)    # 将提取的bit添加到二进制消息中
    encrypted_data = binary_string_to_bytes(binary_message1)

    key2_processed = 0
    binary_key2 = ""  # 信息量二进制串
    for x in range(width):
        for y in range(height):
            pixel = list(encoded_image.getpixel((x, y)))
            for i in range(len(pixel)):
                if (key2_processed >= key1 + 32) and (key2_processed < key1 + 64):
                    extracted_bit = pixel[i] & 0b00000001         # 提取像素的最低1bit
                    binary_key2 += str(extracted_bit)
                key2_processed += 1
    key2 = int(binary_key2, 2)  # 将二进制信息量转化为整数

    message2_processed = 0
    binary_message2 = ""
    for x in range(width):
        for y in range(height):
            pixel = list(encoded_image.getpixel((x, y)))
            for i in range(len(pixel)):
                message2_processed += 1
                if (message2_processed > key1 + 64) and (message2_processed <= key1 + 64 + key2):  # 否则读取字符串过长
                    extracted_bit = pixel[i] & 0b00000001         # 提取像素的最低1bit
                    binary_message2 += str(extracted_bit)    # 将提取的bit添加到二进制消息中
    signature = binary_string_to_bytes(binary_message2)
    return encrypted_data, signature

# 生成RSA密钥对
public_key_a, private_key_a = rsa.newkeys(2048)
public_key_b, private_key_b = rsa.newkeys(2048)

# 加密并签名信息
def encrypt_and_sign_message(message, public_key_receiver, private_key_sender):
    # 使用B的公钥对信息进行加密
    encrypted_data = rsa.encrypt(message, public_key_receiver)

    # 使用A的私钥对信息进行签名
    signature = rsa.sign(encrypted_data, private_key_sender, "SHA-256")

    return encrypted_data, signature

# 验证签名并解密信息
def verify_and_decrypt_message(encrypted_data, signature, public_key_sender, private_key_receiver):
    # 验证签名的有效性
    verification_result = rsa.verify(encrypted_data, signature, public_key_sender)

    if verification_result:
        # 使用B的私钥进行解密
        decrypted_data = rsa.decrypt(encrypted_data, private_key_receiver)
        return decrypted_data
    else:
        print("签名验证失败！")
        return None

if __name__ == "__main__":
    flag = True
    while flag:
        print("*****LSB算法加密/解密*****")
        choice = input("请选择：A、加密  B、解密  C、退出\n")
        if choice == 'A':
            # 读入图像（建议png，否则会被计算机压缩而失去隐藏信息）
            image_path = input("请输入需要加密图片的路径：\n")  # C:/Users/lenovo/Desktop/CQUlibrary.png
            message = input("请输入需要隐藏的信息：\n")  # CQUWATERMASKEXP
            message_bytes = message.encode('utf-8')
            encrypted_data, signature = encrypt_and_sign_message(message_bytes, public_key_b, private_key_a)
            encoded_image = hide_message(image_path, encrypted_data, signature)
            encoded_image_path = input("请输入加密图片保存路径：\n")  # C:/Users/lenovo/Desktop/CQUlibrary_hidden.png
            encoded_image.save(encoded_image_path)
            print("信息隐藏成功！\n")
        
        if choice == 'B':
            encoded_image_path = input("请输入需要解密图片的路径：\n")
            encrypted_data, signature = extract_message(encoded_image_path)
            decrypted_data = verify_and_decrypt_message(encrypted_data, signature, public_key_a, private_key_b)
            if decrypted_data == None:
                print("信息提取失败！\n")
            else:
                print("提取出来的隐藏信息:", decrypted_data)
                print("信息提取成功！\n")
        if choice == 'C':
            flag = False
