#Graphical User Interace for Two Factor Authentication##
#Reference: - https://tutorialspoint.com/pyqt/


import sys
import socket
import time
from PyQt4.QtGui import *
from PyQt4.QtCore import *

HOST = '10.0.0.133'
PORT = 3124
data = "10" 
def window():
   app = QApplication(sys.argv)
   w = QWidget()
   b = QPushButton(w)

   b.setText("Turn Buzzer Off")
   b.resize(200,50)
   b.move(200, 70)
   b.clicked.connect(buzzer_off)

   b1 = QPushButton(w)
   b1.setText("Register new FP")
   b1.resize(200,50)
   b1.move(200, 170)
   b1.clicked.connect(fp)

   b2 = QPushButton(w)
   b2.setText("Delete all FingerPrints")
   b2.resize(200,50)
   b2.move(200, 270)
   b2.clicked.connect(fp_del_all)

   b3 = QPushButton(w)
   b3.setText("Turn Buzzer on")
   b3.resize(200,50)
   b3.move(200, 370)
   b3.clicked.connect(buzzer_on)

   b4 = QPushButton(w)
   b4.setText("Reset Remote Node")
   b4.resize(200,50)
   b4.move(200, 470)
   b4.clicked.connect(reset_remote)

   b5 = QPushButton(w)
   b5.setText("Give Access")
   b5.resize(200,50)
   b5.move(200, 570)
   b5.clicked.connect(give_access)

   w.setGeometry(800,1000,600,700)
   w.setWindowTitle("Control Node GUI")
   w.show()
   sys.exit(app.exec_())
	
def msgbtn(i):
   print "Button pressed is:",i.text()

def buzzer_off():
   num = 1
   s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   s.connect((HOST,PORT))
   s.send(str(num).encode('utf8'))
   s.close()

def fp():
   num = 2
   s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   s.connect((HOST,PORT))
   s.send(str(num).encode('utf8'))
   s.close()

def buzzer_on():
   num = 3
   s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   s.connect((HOST,PORT))
   s.send(str(num).encode('utf8'))
   s.close()

def fp_del_all():
   num = 4
   s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   s.connect((HOST,PORT))
   s.send(str(num).encode('utf8'))
   s.close()

def reset_remote():
   num = 5
   s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   s.connect((HOST,PORT))
   s.send(str(num).encode('utf8'))
   s.close()

def id(text):
   print "id"+text

def give_access():
   num = 6
   s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   s.connect((HOST,PORT))
   s.send(str(num).encode('utf8'))
   s.close()

    	
if __name__ == '__main__': 
   window()






