# Trumpet Renderer GUI toolkit.

import sys
from PyQt6 import uic
from PyQt6.QtCore import *
from PyQt6.QtWidgets import QApplication, QMainWindow
from PyQt6.QtWidgets import QVBoxLayout, QHBoxLayout, QGridLayout, QWidget, QPushButton

from ButtonClass import PushButtonCore

class ButtonAction() :
    def CreateAction() :
        print('create Action')


'''
렌더링 툴의 GUI를 담당할 Python class.
이름은 트럼펫으로 만들 것이다.
'''
class Trumpet (QMainWindow):
    def __init__(self) :
        super().__init__()
        self.initUI()
    
    def initUI(self) :
        self.setWindowTitle("Trumpet")
        self.setGeometry(500,200,300,400)
        
        self.Btn = QPushButton()
        self.Btn.setText('Create')
        self.Btn.clicked.connect(ButtonAction.CreateAction)
        
        self.layout = QVBoxLayout()
        self.layout.addWidget(self.Btn)
        
        widget = QWidget()
        widget.setLayout(self.layout)
        self.setCentralWidget(widget)
    
        
        

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = Trumpet()
    window.show()
    app.exec()