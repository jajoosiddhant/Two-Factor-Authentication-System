import sys
import socket
from PyQt4.QtGui import *
from PyQt4.QtCore import *

HOST = '10.0.0.133'
PORT = 3124
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))
data = "10" 
def window():
   app = QApplication(sys.argv)
   w = QWidget()
   b = QPushButton(w)
   b.setText("Show message!")

   b.move(50,50)
   b.clicked.connect(showdialog)
   w.setWindowTitle("PyQt Dialog demo")
   w.show()
   sys.exit(app.exec_())

def showdialog():
    msg = QMessageBox()
    msg.setIcon(QMessageBox.Information)
    msg.setText("This is a message box")
    msg.setInformativeText("This is additional information")
    msg.setWindowTitle("MessageBox demo")
    msg.setDetailedText("The details are as follows:")
    msg.setStandardButtons(QMessageBox.Ok | QMessageBox.Cancel)
    msg.buttonClicked.connect(socket)
    retval = msg.exec_()
    print "value of pressed message box button:", retval
	
def msgbtn(i):
   print "Button pressed is:",i.text()

def socket():
    s.send("10")
    
	
if __name__ == '__main__': 
   window()






