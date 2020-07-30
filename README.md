# acquisition

# Dépendances

#Installer les dépendances aravis0.5
#Installer aravis0.5

#Installer Opencv

# Installer QT
----------------------------------------------------------------
#clone dépôt ou télécharger le zip

#cd acquisition
#mkdir build

#cmake ..

#make -j4

#Mono camera acquisition : ./acq

#Multi camera acquisition (2 GigE Cam in same time) : ./acqMulti

---------------------------------------------------------------

# Mono camera

1 fenêtre d'acquisition s'ouvre. On peut paramétrer le répertoire d'enregistrement, sélectionner la caméra à utiliser pour l'acquisition.
On peut également modifier la fréquences d'acquisition de la caméra (différent du nombre de frame par secondes). Par défaut elle est de 5 hz (images saccadés). Peut monter jusqu'à 25 hz, peut être plus à tester.
On peut également paramétrer le delais d'acquisition entre les paquets de la caméra par défaut 25. Ce paramètre n'impacte pas l'acquisition monocamera .

On appuie sur le bouton start et l'acquisition démarre, le fichier vidéo au format avi est enregistré dans le répertoire sélectionné.
