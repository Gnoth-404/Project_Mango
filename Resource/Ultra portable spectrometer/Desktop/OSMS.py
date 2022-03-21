#
#The OSMS software was designed by Katrina Laganovska, University of Latvia, Institute of Solid State Physics. 
#If you have any questions, comments or feedback, please contact me at katrina.laganovska@cfi.lu.lv.
#



import serial #for Serial communication
import time   #for delay functions
import math
import random
import sys
import pyqtgraph as pg
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QWidget, QInputDialog, QLineEdit
from PyQt5.QtCore import QObject, pyqtSlot
from PyQt5.QtGui import QPen, QFileDialog, QInputDialog


import serial.tools.list_ports;# print([comport.device for comport in serial.tools.list_ports.comports()])
ports=serial.tools.list_ports.comports()
testst=list(map(str, ports))
print(testst[0][:4])



arduino = serial.Serial(testst[0][:4],115200) #Create Serial port object called arduinoSerialData
arduino.flushInput() #clear any previous data
time.sleep(2) #wait for 2 secounds for the communication to get established
arduino.write(str.encode('3')) 

nm=[]
baseline=[]
spectrum=[]
baseline2=[]
spectrum2=[]
absorption=[]
nmPlot=[]
absorPlot=[]
accum=1
average=0

for i in range (0,288): #initilizing arrays
    nm.append(i)
    baseline.append(i)
    baseline[i]=0
    spectrum.append(i)
    baseline2.append(i)
    baseline2[i]=0
    spectrum2.append(i)
    spectrum2[i]=0
    absorption.append(i)

    #calculating wavelengths from the formula provided by manufacturer
    k = (3.103932661*math.pow(10,2)+2.683934106*i-1.098262279*math.pow(10,-3)*math.pow(i,2)-7.817392551*math.pow(10,-6)*math.pow(i,3)+9.609636190*math.pow(10,-9)*math.pow(i,4)+4.681760466*math.pow(10,-12)*math.pow(i,5));
    nm[i]=k 

for i in range (53,198): #still initializing arrays
    absorPlot.append(i)
    nmPlot.append(i)

class Ui_MainWindow(QWidget):

    def setupUi(self, MainWindow): #holds the GUI
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(795, 522)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.Single = QtWidgets.QPushButton(self.centralwidget)
        self.Single.setGeometry(QtCore.QRect(270, 440, 130, 28))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.Single.sizePolicy().hasHeightForWidth())
        self.Single.setSizePolicy(sizePolicy)
        self.Single.setMinimumSize(QtCore.QSize(130, 28))
        self.Single.setMaximumSize(QtCore.QSize(120, 28))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.Single.setFont(font)
        self.Single.setObjectName("Single")
        self.Single.clicked.connect(self.Capture)
        self.graphicsView = pg.PlotWidget(self.centralwidget) #uses pyqtgraph to display results
        self.graphicsView.setGeometry(QtCore.QRect(10, 10, 771, 421))
        self.graphicsView.setMinimumSize(QtCore.QSize(771, 421))
        self.graphicsView.setXRange(440, 760, padding=0)
        self.graphicsView.setYRange(-0.1, 2.1, padding=0)
        self.graphicsView.setAutoFillBackground(True)
        self.graphicsView.setFrameShape(QtWidgets.QFrame.Box)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.NoBrush)
        self.graphicsView.setBackgroundBrush(QtCore.Qt.white)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.NoBrush)
        self.graphicsView.setForegroundBrush(brush)
        self.graphicsView.setObjectName("graphicsView")
        self.graphicsView.showGrid(x=True, y=True, alpha=0.3)
        self.graphicsView.showMaximized()
        self.STOP = QtWidgets.QPushButton(self.centralwidget)
        self.STOP.setGeometry(QtCore.QRect(720, 440, 60, 28))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.STOP.sizePolicy().hasHeightForWidth())
        self.STOP.setSizePolicy(sizePolicy)
        self.STOP.setMinimumSize(QtCore.QSize(60, 28))
        self.STOP.setMaximumSize(QtCore.QSize(60, 28))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.STOP.setFont(font)
        self.STOP.setObjectName("STOP")

        self.Continuous = QtWidgets.QPushButton(self.centralwidget)
        self.Continuous.setGeometry(QtCore.QRect(410, 440, 130, 28))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.Continuous.sizePolicy().hasHeightForWidth())
        self.Continuous.setSizePolicy(sizePolicy)
        self.Continuous.setMinimumSize(QtCore.QSize(130, 28))
        self.Continuous.setMaximumSize(QtCore.QSize(120, 28))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.Continuous.setFont(font)
        self.Continuous.setObjectName("Continuous")
        self.Continuous.clicked.connect(self.CaptureCont)
        self.progressBar = QtWidgets.QProgressBar(self.centralwidget)
        self.progressBar.setGeometry(QtCore.QRect(10, 440, 118, 21))
        self.progressBar.setProperty("value", 0)
        self.progressBar.setObjectName("progressBar")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 795, 21))
        self.menubar.setObjectName("menubar")
        self.menu_File = QtWidgets.QMenu(self.menubar)
        self.menu_File.setObjectName("menu_File")
        self.menu_Setup = QtWidgets.QMenu(self.menubar)
        self.menu_Setup.setObjectName("menu_Setup")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.actionSave_As = QtGui.QAction("&Save File", self)
        self.actionSave_As.setShortcut("Ctrl+S")
        self.actionSave_As.setStatusTip('Save File')
        self.actionSave_As.triggered.connect(self.saveFileDialog)
        self.actionMeasure_Baseline = QtWidgets.QAction(MainWindow)
        self.actionMeasure_Baseline.setObjectName("actionMeasure_Baseline")
        self.actionMeasure_Baseline.triggered.connect(self.Baseline_meas)
        self.actionAbsorption_at_Single_Wavelength = QtWidgets.QAction(MainWindow)
        self.actionAbsorption_at_Single_Wavelength.setObjectName("actionAbsorption_at_Single_Wavelength")
        self.actionAccumulation = QtWidgets.QAction(MainWindow)
        self.actionAccumulation.setObjectName("actionAccumulation")
        self.actionAccumulation.triggered.connect(self.AccumulationDialog)
        self.menu_File.addAction(self.actionSave_As)
        self.menu_Setup.addAction(self.actionMeasure_Baseline)
        self.menu_Setup.addAction(self.actionAccumulation)
        self.menu_Setup.addAction(self.actionAbsorption_at_Single_Wavelength)
        self.menubar.addAction(self.menu_File.menuAction())
        self.menubar.addAction(self.menu_Setup.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)
        

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "OSMS"))
        self.Single.setText(_translate("MainWindow", "Single"))
        self.STOP.setText(_translate("MainWindow", "STOP"))
        self.Continuous.setText(_translate("MainWindow", "Continuous "))
        self.menu_File.setTitle(_translate("MainWindow", "&File"))
        self.menu_Setup.setTitle(_translate("MainWindow", "&Measurements"))
        self.actionSave_As.setText(_translate("MainWindow", "Save As"))
        self.actionMeasure_Baseline.setText(_translate("MainWindow", "Measure Baseline"))
        self.actionAbsorption_at_Single_Wavelength.setText(_translate("MainWindow", "Absorption at Single Wavelength"))
        self.actionAccumulation.setText(_translate("MainWindow", "Accumulation "))
        
    def AccumulationDialog(self):
        integer, ok=QtWidgets.QInputDialog.getInt(self, "Enter count","")
        global accum
        accum=integer
        

    def saveFileDialog(self): #opens SaveAs dialog and allows to save the already existing absorption file under whichever name the user chooses
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getSaveFileName(None, 'Save As', '','Text Files (*.txt)', options=options)#selectedFilter='*.txt')
        with open(fileName+".txt",'w') as f:
            with open("absorption.txt", 'r') as f1:
                f.write(f1.read())
                f1.close()
            f.close()


    def update_plot(self): #updates the plot after receiving absorption values
        
        self.graphicsView.clear()
        self.graphicsView.setYRange(0,ylimit)
     #   color1=random.randint(1,255)
      #  color2=random.randint(1,255)
       # color3=random.randint(1,255)
        self.graphicsView.plot(nmPlot,absorPlot,pen=pg.mkPen(color=(0,0,0)))
        

    def Baseline_meas(self): #measure the baseline (the spectrum of the light source without the sample)
        
        
        progress=0
        self.progressBar.setValue(0)
        QtCore.QCoreApplication.processEvents()
        for i in range (0,288):
            baseline[i]=0

        for i in range (0,accum):
          #  lbl2 = Label(window, text=i)
           # lbl2.grid(column=0, row=1)
           # window.update()
            arduino.write(str.encode('4'))  
            #time.sleep(5)
            msg=arduino.readline()  
            #time.sleep(5)
            msg=msg.decode().split(',')
            msg=msg[:-1]
            msg=list(map(int, msg))
            
            for i in range (0,288):
                baseline[i]+=msg[i]
            progress=progress+1
            progresspercent=progress/accum*100
            self.progressBar.setValue(progresspercent)

            QtCore.QCoreApplication.processEvents()
                      

        open("baseline.txt", "w+") #opens the file baseline.txt (located in the same place as this script) and saves the measured values
        with open("baseline.txt", "a") as outF:
            for i in range (1,288):
                outF.write(str(baseline[i])+'\n')
            outF.close()

 
    def Capture(self):
        progress=0
        self.progressBar.setValue(0)
        QtCore.QCoreApplication.processEvents()
        for i in range (0,288):
                spectrum[i]=0
        

        for i in range (0,accum):

            arduino.write(str.encode('5'))  
            msg=arduino.readline()  
            msg=msg.decode().split(',')
            msg=msg[:-1]
            msg=list(map(int, msg))
            
            for i in range (0,288):
                spectrum[i]+=msg[i]
            
            progress=progress+1
            progresspercent=progress/accum*100
            self.progressBar.setValue(progresspercent)
            QtCore.QCoreApplication.processEvents()


        open("spectrum.txt", "w+")
        with open("spectrum.txt", "a") as outS:
            for i in range (1,288):
                outS.write(str(spectrum[i])+'\n')
            outS.close()
            #   print(nm[i], file="baseline.txt")
        global ylimit
        ylimit=0
        for i in range (53,198):
            baseline2[i]=(baseline[i]+baseline[i-1]+baseline[i+1])/3-980*accum
            if(baseline2[i]<=0): 
                baseline2[i]=1
            spectrum2[i]=(spectrum[i]+spectrum[i-1]+spectrum[i+1])/3-980*accum
            if(spectrum2[i]<=0): 
                spectrum2[i]=1

            absorption[i]=math.log(baseline2[i]/spectrum2[i],10)
            average=+absorption[i]

        average=average/(198-53)

    #uncomment these two lines and comment out the next one to save data continuously    
    #    mytime=time.strftime('%j%H%M%S')
    #    outA=open("absorption"+str(mytime)+".txt", 'w')

        outA=open('absorption.txt', 'w')
        
        #with open("absorption.txt", "a") as outA:
        for i in range (53,198):
            absorption[i]=absorption[i]+0.0556-0.0767*average
            if(absorption[i]>ylimit):
                ylimit=absorption[i]
                
            outA.write(str(nm[i])+'\t'+str(absorption[i])+'\n')
        outA.close()


        for i in range (53,198):
            absorPlot[i-53]=absorption[i]
            nmPlot[i-53]=nm[i]
        self.update_plot()
       # self.graphicsView.plot(nmPlot,absorPlot)

    def CaptureCont(self):
        
        global breaktheloop
        breaktheloop=0
        self.Capture()

        self.STOP.clicked.connect(self.breakloop)
        QtCore.QCoreApplication.processEvents()   
        if(breaktheloop==0):
            self.CaptureCont()     

        #set a delay time between measurements if necessary. This would be a delay of 10 seconds
        #time.sleep(10)

    def breakloop(self):
        global breaktheloop
        breaktheloop=1

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)    
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())


