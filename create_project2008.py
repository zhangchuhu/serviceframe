#!/usr/bin/env python
# -*- coding: gb2312 -*-

import os
import codecs
import shutil
from xml.dom import minidom

# �����ļ������Դ����Ŀ¼��·��
out_file = "./serviceframe_2008/serviceframe.vcproj"

# Դ����Ŀ¼
work_dir = "./"

# ���Ե�Ŀ¼
igone_files = [".git", "doc", "serviceframe_2008"]

# ���Ե��ļ�
igone = [
	"tags", "noblocking_thrift_client.h", "NoBlockThriftCilent.h",
]



"""
����VC�����ļ�
"""

class VSProj:
    def __init__(self):
        self.doc = minidom.Document()
        self.root_node = self.doc.createElement("Files")
        self.doc.appendChild(self.root_node)

    def addDir(self, path, father_node=None):        
        filter_node = self.doc.createElement("Filter")
        filter_node.setAttribute("Name", os.path.basename(path).decode('gb2312'))
        #filter_node.setAttribute("Filter", "")
        file_list = []
        for f in os.listdir(path):
            if f == ".svn":
                continue
            rel_path = os.path.join(path, f)                                
            if os.path.isdir(rel_path) == True:
                #self.addDir(rel_path, filter_node)                         
                file_list.append(rel_path)
            else:
                self.addFile(rel_path, filter_node)                         
                

        # ��ӱ�Ŀ¼���ļ���
        file_list.sort()
        for f in file_list:
            self.addDir(f, filter_node)
            
        if father_node == None:
            self.root_node.appendChild(filter_node)
        else:
            father_node.appendChild(filter_node)

    def addFile(self, path, father_node=None):
        global out_file
        if os.path.split(path)[-1] in igone:
            return
        k_rel_path = os.path.split(out_file)[0]                             # �����ļ�����Ŀ¼
        file_node = self.doc.createElement("File")
        rel_path = os.path.relpath(path, k_rel_path)                        # ���vip_/vip�������ļ�����Ŀ¼����·�� ../../vip_score_v2/xx
        file_node.setAttribute("RelativePath", rel_path.decode('gb2312'))
        if father_node == None:
            self.root_node.appendChild(file_node)
        else:
            father_node.appendChild(file_node)

    def genXml(self, out_file):
        bak_file = out_file + ".bak"
        shutil.copy(out_file, bak_file)                # bak file
        fd = open(out_file, "w")
        fd2 = open(bak_file, "r")
        flag = False                              #
        while True:
            line = fd2.readline()
            if line == "":
                break
            elif line.find('<Files>') != -1:
                flag = True
                str = self.doc.toprettyxml(indent="\t", newl="\n", encoding="gb2312")
                #print "fild start: %s" % str
                pos = str.find("\n")
                fd.write(str[pos+1:])                   # ȥ��xmlͷ
            elif line.find('</Files>') != -1:
                #print "fild end: "
                flag = False
            elif flag:
                continue
            else:
                fd.write(line)
        
        fd.close()
        fd2.close()
        """
        f = file(file_name, "w")
        writer = codecs.lookup('gb2312')[3](f)
        self.doc.writexml(writer, encoding='gb2312')    #  ����xml��ʽ
        writer.close()
        """
        

if __name__ == "__main__":    

    os.chdir(work_dir)

    # begin gen xml
    xl = VSProj()
    file_list = []
    for i in sorted(os.listdir("./")):
        if igone_files.count(i) != 0:
            continue        
        rel_path = os.path.join("./", i)
        #print rel_path
        if os.path.isdir(rel_path) == False: 
            xl.addFile(rel_path)
        else:
            file_list.append(rel_path)

    # �ȼ��ļ��У��ڼ��ļ���
    file_list.sort()
    for i in file_list:
        xl.addDir(i)
    
    xl.genXml(out_file)


