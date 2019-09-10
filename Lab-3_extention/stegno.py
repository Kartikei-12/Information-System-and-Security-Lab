
import sys
import numpy as np

from math import log2, ceil
from PIL import Image

COLOR = 0
MAX_SIZE_BITS = 20

def to_bits(s):
    result = list()
    if isinstance(s, int):
        while s!=0:
            result.append(s%2)
            s //= 2
        result.extend([0 for _ in range(MAX_SIZE_BITS-len(result))])
        result.reverse()
    else:
        for c in s:
            bits = bin(ord(c))[2:]
            bits = '00000000'[len(bits):] + bits
            result.extend([int(b) for b in bits])
    return result

def from_bits(bits):
    chars = []
    for b in range(int(len(bits) / 8)):
        byte = bits[b*8:(b+1)*8]
        chars.append(chr(int(''.join([str(bit) for bit in byte]), 2)))
    return ''.join(chars)

def encrypt(image, plain):
    index = 0
    image = image.copy()
    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            value = plain[index]
            if (value == 1 and image[i][j][COLOR]%2 == 0) or \
               (value == 0 and image[i][j][COLOR]%2 == 1):
                image[i][j][COLOR] += 1
            if image[i][j][COLOR] == 256:
                image[i][j][COLOR] = 254
            index += 1
            if index >= len(plain):
                return image

def decrypt(image):
    i, j = 0, 0
    result = list()
    size_bits = list()
    for _ in range(MAX_SIZE_BITS):
        size_bits.append(image[i][j][COLOR]%2)
        j += 1
        if j == image.shape[1]:
            j = 0
            i += 1
    size = 0
    size_bits.reverse()
    for ii in range(len(size_bits)):
        size += size_bits[ii] * (2 ** ii)
    for ii in range(size*8):
        result.append(image[i][j][COLOR]%2)
        j += 1
        if j == image.shape[1]:
            j = 0
            i += 1
    return from_bits(result)

def save_image(name, image):
    Image.fromarray(image).save(name)

def load_image(name):
    return np.array(Image.open(name))

def main(input_file, message, to_encrypt=False, to_decrypt=False):
    if to_encrypt and ceil(log2(len(message))) > MAX_SIZE_BITS:
        raise ValueError("Message size too long")
    if not to_decrypt and not to_encrypt:
        print('No operation performed')
        return
    if to_encrypt:
        image = load_image(input_file)
        m_len = to_bits(len(message))
        message = to_bits(message)
        m_len.extend(message)
        cipher = encrypt(image, m_len)
        save_image('encrypt_'+input_file, cipher)
    # print("-------------------------------")
    elif to_decrypt:
        plain = decrypt(load_image(input_file))
        print("Secret Message:", plain)

if __name__ == "__main__":
    file = input('Enter image file: ')
    choice = int(input('1. Encryption\n2. Decryption\n\t Choose: '))
    if choice == 2:
        main(file, '', to_decrypt=True)
    else:
        msg = input('Enter message file: ')
        main(file, open(msg).read(), to_encrypt=True)
