#-*-coding:utf-8 -*-
import  os
import xlwt

#从头文件中取出宏定义行
file_source = open('./config/product_config_drv.h')
file_dst = open('./define_dst1.txt','w')
#file_dst.write('#include "product_config_drv.h"'+'\n')
list1 = []
while 1:
    line = file_source.readline()
    if not line:
        break
    if line.startswith('#define'):
        file_dst.write(line)
        file_dst.write("\n")
file_source.close()
file_dst.close()

#删除空行
file1 = open('./define_dst1.txt')
file2 = open('./define_dst2.txt','w')
for line in file1.readlines():
    if line == '\n':
        continue
    file2.write(line)
file1.close()
file2.close()

#取DDR
file3 = open('./define_dst2.txt')
file4 = open('./define_dst3.txt','w')
#动态变化处
file4.write('#include "product_config_drv.h"'+'\n')
while 1:
    line1 = file3.readline()
    if not line1:
        break
    list = line1.split(' ')
    for i in list:
        if i.startswith('DDR'):
            file4.write(i+'\n')
file3.close()
file4.close()

#调用cpp命令生成一个地址文件
#同时，这也是一个动态变化处
os.system('cpp ./define_dst3.txt -I ./config/ -o dizhi.txt')

#删掉地址文件中的无关信息
file5 = open('./dizhi.txt')
file6 = open('./dizhi1.txt','w')
while 1:
    line2 = file5.readline()
    if not line2:
        break
    if not line2.startswith('#'):
        file6.write(line2)
file5.close()
file6.close()

#删掉目标文件的第一行
file7 = open('./define_dst3.txt')
file8 = open('./define_dst4.txt','w')
while 1:
    line3 = file7.readline()
    if not line3:
        break
    if not line3.startswith('#'):
        file8.write(line3)
file7.close()
file8.close()

#将地址文件的值全部转换为16进制
file9 = open('./dizhi1.txt')
file10 = open('./dizhi2.txt','w')
while 1:
    line4 = file9.readline()
    if not line4:
        break
    k = hex(eval(line4))
    if len(k) != 10:
        length = 10-len(k)
        list_tmp1 = k[0:2]
        list_tmp2 = k[2:] 
        file10.write(list_tmp1+'0'*length+list_tmp2+'\n')
        continue
    file10.write(k+'\n')
file9.close()
file10.close()

#将项目名称文件和地址文件导入到最终文件
file11 = open('./define_dst4.txt')
file12 = open('./dizhi2.txt')
file13 = open('./dst.txt','w')
file13.write('项目名称 大小(HEX） BEGIN_ADDR END_ADDR'+'\n')
dic = {}
list_size = []
while 1:
    line5 = file11.readline()
    line6 = file12.readline()
    if not line5:
        break
    if not line6:
        break
    line5_new = line5.replace('\n','')
    if line5_new.endswith('SIZE'):
        list_size.append(line5_new)
    line6_new = line6.replace('\n', '')
    dic[line5_new] = line6_new
for i in list_size:
    str = i.replace('SIZE','ADDR')
    file13.write(i+' ')
    file13.write(dic[i] + ' ')
    if str in dic:
        file13.write(dic[str]+' ')
        hex_num = hex(eval((dic[str]))+eval((dic[i])))
        if len(hex_num) != 10:
            hex_str1 = hex_num[0:2]
            hex_str2 = hex_num[2:]
            lenth_hex = 10 - len(hex_num)
            str_new_hex = hex_str1+'0'*lenth_hex+hex_str2
            file13.write(str_new_hex+'\n')
            continue
        file13.write(hex_num+'\n')
    else:
        file13.write('null'+' ')
        file13.write('null'+'\n')
file11.close()
file12.close()
file13.close()

#将文本文件的内容写入excel中
file = open('./dst.txt')
workbook = xlwt.Workbook()
worksheet = workbook.add_sheet('sheet1')
a = -1
while 1:
    line_data = file.readline()
    if not line_data:
        break
    a=a+1
    q=0
    list = line_data.split(' ')
    for b in list:
        worksheet.write(a,q,b)
        q=q+1
path = os.getcwd()
list_path = path.split('/')
file_name = list_path[len(list_path)-1]
print(file_name)
workbook.save(file_name+'_内存布局划分.xls')
file.close()

#调用系统命令删除中间文件
os.system('rm define_*')
os.system('rm dizhi*')
os.system('rm dst.txt')