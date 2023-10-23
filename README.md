# FileIndexer
File Indexer is a cross-platform desktop program for indexing files in a given directory. This application was developed using the c++ programming language, the QT6 GUI development library, and the qml declarative programming language.# What this app can do

## Requirments

```
CMake 3.16
C++17
QT 6
```

## Install dependencies
### Arch
Install CMake:
```
sudo pacman -S cmake
```
Install QT 6:
```
sudo pacman -S qt6-base
```
Install git:
```
sudo pacman -S git
```
### Ubuntu
Install CMake:
```
sudo apt install cmake
```
Install QT 6
```
sudo apt install qt6-base-dev
```
Install git
```
sudo apt-get install git-all
```
## How to build a project
Clone repo from github:
```bash
cd ~/Downloads
git clone git@github.com:hiddensudo/FileIndexer.git
cd FileIndexer
```
Build FileIndexer:
```bash
mkdir build
cd build
cmake ..
make
```
To launch the application, you can use the command `make run` in the build directory

## Indexing in the specified directory
You can start searching for files in the specified directory, and when all the files are found, they will appear in the table to the right of the menu.

![IndexInCurrentDir](https://github.com/hiddensudo/FileIndexer/assets/111019096/825a2531-871b-4b7b-9710-7bc4f38fae85)

## Indexing in the specified directory and subdirectories
You can also index files in a directory and all subdirectories. This can be time-consuming, so at any time you can pause this function or stop it altogether. If you pause the indexing, you can resume it at any time and continue it until it is finished or until you stop it. After indexing is complete, you will see information about all the files in the table.

![FileIndexerIndexing](https://github.com/hiddensudo/FileIndexer/assets/111019096/a7a85068-2ccc-475e-822b-9adcab725923)

## Find in index

### Find by extension
You can find more detailed information about your indexed files if you know at least part of the name, extension, last modified date, or file size. To find all files with an extension, just select an extension from the menu of more than a hundred available ones. 

![FileIndexerFinding](https://github.com/hiddensudo/FileIndexer/assets/111019096/ad1ab8e3-351f-4bf0-93b9-670990c77661)

### Find by date
If you want to search by date, name, or size, you need to select the search criterion in the corresponding drop-down menu, then enter the relevant information in the field and click the "Find" button.

![FindByDate](https://github.com/hiddensudo/FileIndexer/assets/111019096/c8dcfdcd-8932-4059-b05c-74ad461febeb)

### Find by size

![FindBySize](https://github.com/hiddensudo/FileIndexer/assets/111019096/e399fc90-b0d6-4cf6-adae-fc202cce81c8)

## Uml Class Diagram
### About UML Class Diagram
While working on this application, an UML class diagram was developed. To view it in detail, you can open the file called UmlClassDiagram.puml

### UML Class Diagram Visualization
To visually represent a Uml class diagram, you can use the resource [www.plantuml.com](www.plantuml.com).

To do this, follow [this link](https://www.plantuml.com/plantuml/uml/SyfFKj2rKt3CoKnELR1Io4ZDoSa70000) and paste the code from the file UmlClassDiagram.puml. Or copy this URL
```
//www.plantuml.com/plantuml/png/dLPFTnet4B_pAUR4Msnyw9Mal61CNsdZE6kSD3S_UJK6jLffAsaTY6D_zcgqYnOMrdxk1NPcVlFJ_D58-c0TCwxCPI-Jp5gut4oA0Xvx06TfcailI94n9xIQgQLG21Z-o3vN7DTeG5J_fFfDI7cfEKguttwH-a4B3XPTMKoSC-AkT6YJmRwbeczhfrefp6Zvok4qn9S8vQ3Fp38RGhPY1avElF0G06GofKe6y0Z6KnW5Fhei6Ro3fzvJhqvpMbgdyuLZYZF3vmxp4EnZBxszElzZDhtfdQ8gSxZM4ctuWKUV7Cuv2V0hf6uq-cBH1F4KVXd29vOZIKEOhHqgwsEkn0lcQiD2_0XVyFJEyw-fjC9rnPAaLtT_O-RWf606bVEBgbBAmfc1zqw_CYEOrtBcM99fPHsamI-q1VgXAqC8vT0I1r1Xz0CQ8pWInU_Chjvxm13Iyut3YJ5iCmveoiKcXpwnVasZTqfsbbA9gFKUUjQH5fpazES1eSkwG4LUoM2hoQIsEw9Ftc90fDzokN06MNwDZAE1_ZgNGwXpznQXbj3FZ71e13imE5ox0oLkMqKrZKab93tlY-cHJ9ReQa35ws5DpDykFv9Zq51D79haRR4UZJAse1w_tGRGqDO-lZIVcLkHoWVoLrW8lXkIhUEZaQrWOVelXC7CQRCXwvtM4eJzR7I6rdhCN4rBOr0HBBeptrgHlIzzL4k5dBRP65Zcn0FUh0moRjjGuXqJCIijyYxhbAaCfUm6B2ZW7NDU-bf1GQINz1aD_vPOullmIHlIRgp3V3GgV576KCJiKeArsCYrnURxmmKTGxSFD7Tt4YlKz6lBDunTzANULwgvykMRABuex_n7lH5sEz7FOfMhPWnQ1B4DRtMlavr8cHme8_n_KORY9M_peL6svypYbLfaki2tBVoTDl9S-PCW-hKQpBTEjvx1RA0V_WPnQpkzN4eCm-FNga9e-0tU6xIhHbDDo1OUzAVrv5N2SvJy0StndfNIaJwzdN_wUlNdvFpZ308HHyUorNMfsgV0_WmEEuSlB1ToMAARRoTXU_Ri6sdAkcnnnkAnzPC056GpODCLKqk2DAP0UjmIAG-DOJHtP8Esp7SLrkd2Y_5SQuuhUeNGaNQXymkTUI-wxkgFTxKjtZYredsfjBJp-zbQM1SDn3sJI8nKd6kjNRizqkFfFXp2NY3zRIJ7g3A3p265K6lQf8e8Eo-ZOQFZp1q2hVZnQZZDkDpeK8kNuFLiP2lC_fae_gNWTVXlZBjjUJdwMAw-qVg5Ejxu1JhgwDYIUjDrWRwUiWW7KjWKjhlpBGFjDpBKJATnooQl09yRv5Qrz-Q9gdZI7Mb2LrZ7UjFPccETeSPKBupmoeQpit5yPOoWj6XXt9YQDs1ZhD7U_7CCzVkr5t7Twd7N8xYXxW3Kd0Vw60gHnwetT1zGyJAN_m40
```
and paste it into the input for the URL, and click Decode URL

## Modules
### Indexer module
The indexer module is responsible for indexing the file. This module is used in the program to navigate through directories. The writeInXml module is used when navigating through directories.

### Write in xml module
WriteInXml module is used to write new information about newly indexed files. When writing data to an xml file, it checks whether the file information has already been written. If the information has already been written, the file will not be written twice.

### FillModel module
The FillModel module is used to fill in the table. At the start of the program, this module fills the table with data that is already insulated in the xml file. This module also provides table updates and filtering in the program.
