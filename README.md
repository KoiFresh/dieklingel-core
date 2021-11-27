# dieklingel-core
![alt text](https://dieklingel.com/images/logo_2500x500_free.png)
dieKlingel ist ein Projekt unter der Leitung von Kai Mayer und Sven Schoch. Alle Informationen zum Projekt können unter <https://dieklingel.com/> entnommen werden. 

#### Installation
Das komplette Projekt setzt sich aus mehreren Komponenten zusammen. Hierzu Zählen:
1. dieklingel-core: Dienst als Basis der Anwendung. Enthalten sind der Core, welcher auf der DoorUnit läuft und eine Beispielkonfiguration einer Graphioschen Anwendung (nur im Debian Packet enthalten).   
2. dieklingel-app: Eine App, erhältlich für iOS und Android um mit der Basis zu kommunizieren. 
3. ct-server: Der ConnectionTool-Server dient als Gateway für die Kommunikation zwischen App und Basis 

Um dieKlingel auf einer DoorUnit zu installieren gibt es zwei Möglichkeiten "1. Installation durch eine vorhandenes Debian Packet" oder "2. Installation aus 
dem Quellcode". Vor jeder Installation sollte die entsprechende Umgebung eingerichtet werden.

Hierzu zählen unter anderem folgende Komponenten:
1. X11 Server / Desktopumgebung - Hilfe zur einrichtung finden Sie hier: https://dieklingel.com/software/?page=config#x11
2. Pulseaudio Server - Hilfe zur einrichtung finden Sie hier: https://dieklingel.com/software/?page=config#pulseaudio
3. Benutzer der Desktopumgebung - im Normalfall "dieklingel"
4. Benutzer der Graphischen anwendung - im Noramlfall "root"

Bevor mit der Installation begonnen wird, sollte die Umgebung vollständig eingreichtet sein. 

##### 1. Installation durch ein vorhandenes Debian Packet

Laden Sie die Version 1.0-1 herunter: 

```bash
wget https://dieklingel.com/software/download/$(dpkg --print-architecture)/dieklingel_latest_$(dpkg --print-architecture).deb
```
und installieren Sie diese:

```bash
sudo apt-get update && sudo apt-get install ./dieklingel_latest_$(dpkg --print-architecture).deb
```

nach der Installation üperprüfen Sie ob der gewählte Benutzer zu Ihrem System passt
Dazu öffnen Sie mit einm Texteditor Ihrer Wahl folgende Datei  ``` sudo nano /etc/systemd/system/dieklingel-gui.service``` und änderen Gegebenenfalls die Zeil ``` user=pi``` in den 
von Ihnen gewählten Benutzer. 

Nun Laden Sie die Datei neu:
```bash
sudo systemctl daemon-reload
```
Um die Anwendung bei jedem start automatisch zu starten, muss dieser Befehl ausgeführt werden:
```bash
sudo systemctl enable dieklingel 
```
Um die Graphische Anwendung bei jedem start automatisch zu starten, muss dieser Befehl ausgeführt werden:
```bash
sudo systemctl enable dieklingel-gui 
```

**dieklingel-core** ist vollständig installiert und kann nun Konfiguriert werden. 

##### 2. Installation aus dem Quellcode mit VSCode
kommt bald ...

##### 2. Installation aus dem Quellcode
(veraltet) 
Die vorbereitung um dieklingel-core aus dem Quellcode zu bauen sind die Selben wie durch die Installation durch ein vorhandenes Debian Packet.

Installieren Sie abhägingkeiten, welche zur Kompilierung notwendig sind:
```bash
sudo apt-get update && sudo apt-get install -y git qt5-default liblinphone-dev libcurl4-openssl-dev libopencv-dev libcrypto++-dev libcrypto++-doc libcrypto++-utils build-essential libncurses5-dev make libncursesw5-dev flex bison libssl-dev bcc qtmultimedia5-dev libqt5websockets5-dev qtdeclarative5-dev qtquickcontrols2-5-dev wiringpi zip chromium-browser
```
Als nächstes Klonen Sie dieses Git-Repo und wechseln in das Verzeichnis
```bash
git clone https://github.com/KoiFresh/dieklingel-core.git
mkdir dieklingel-core/src/build && cd dieklingel-core/src/build
```
Nun können Sie mit dem Kompilieren beginnen:

Bereitstellen mit GPIO Unterstützung (nur für RaspberryPi):
```bash
qmake DEFINES+=WIRINGPI ../
```
Bereitstellen ohne GPIO Unterstützung:
```bash
qmake ../
```
Kompilieren
```bash
make
sudo make install
```

nach der Installation üperprüfen Sie ob der gewählte Benutzer zu Ihrem System passt
Dazu öffnen Sie mit einm Texteditor Ihrer Wahl folgende Datei ``` sudo nano /etc/systemd/system/dieklingel-gui.service``` und änderen Gegebenenfalls die Zeil ``` user=pi``` in den 
von Ihnen gewählten Benutzer. 

Nun Laden Sie die Datei neu:
```bash
sudo systemctl daemon-reload
```

**dieklingel-core** ist vollständig installiert und kann nun Konfiguriert werden. 

##### Konfiguration
 
eine Erklärung zu Konfiguration der Basis finden Sie unter https://dieklingel.com/software/?page=config
