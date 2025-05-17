import sys
from PyQt6.QtWidgets import QApplication, QWidget, QPushButton

class PushButtonCore(QPushButton) :
    def __init__(self):
        super().__init__()
        self.SetupUI()
        
    def SetupUI(self):
        coreButton = QPushButton("button", self)
        coreButton.setCheckable(True)