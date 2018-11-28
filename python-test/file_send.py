# send

import os
import hashlib
import struct
import json

BUFFER_SIZE = 1024
HEAD_STRUCT = '128sIq32s'


def cal_md5(file_path):
    with open(file_path, 'rb') as fr:
        md5 = hashlib.md5()
        md5.update(fr.read())
        md5 = md5.hexdigest()
        return md5


def get_file_info(file_path):
    file_name = os.path.basename(file_path)
    file_name_len = len(file_name)
    file_size = os.path.getsize(file_path)
    md5 = cal_md5(file_path)
    return file_name, file_name_len, file_size, md5


def send_file(sock,file_path):

    file_name, file_name_len, file_size, md5 = get_file_info(file_path)
    file_head = struct.pack(HEAD_STRUCT, file_name.encode('utf-8'), file_name_len, file_size, md5.encode('utf-8'))

    try:
        sock.send((json.dumps({"down-files":"ready"}) + "\n").encode('utf-8'))
        sock.send(file_head)
        print("send file head")
        sent_size = 0

        with open(file_path) as fr:
            while sent_size < file_size:
                remained_size = file_size - sent_size
                send_size = BUFFER_SIZE if remained_size > BUFFER_SIZE else remained_size
                send_file = fr.read(send_size)
                sent_size += send_size
                sock.send(send_file.encode())
                print("send file ",send_file.encode())
    except Exception as e:
        print ("Socket error: %s" % str(e))

