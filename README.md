# -hardware-_S172
Logiciels Stage Zéphyr

### Documentation
Toute la documentation sur Zéphyr se trouve dans le fichier [Zephyr_documentation.docx](Zephyr_documentation.docx).

Pour System View, la documentation est directement dans le projet `-hardware-_S137_asf` dans le dossier `S137/peripherals_SAMD21/SystemView` avec le code source directement.

### Poc BareMetal
Dans le dossier `My Project` se trouve le POC en BareMetal utilise le capteur de température et luminosité ainsi que la carte SD. Ce POC fonctionne sur nxg_court et implémente aussi la basse consommation.

### Work Queue
Sur la branche `work_queue` le même POC est présent, mais cette fois la work queue est utilisé dans la boucle main pour pouvoir faire plusieurs choses en même temps et éviter les delays. La work queue est instrumenté avec System View, mais si la work queue tourne trop rapidement la plupart des messages vont être perdu, car RTT ne sera pas assez rapide pour les lire. Sur System View ça se verra avec un bandeau rouge signalant un overflow. System View par défaut skip (mets à la poubelle) les messages quand le buffer RTT est plein. Ce comportement est écrit en dur dans le code source de SystemView, et ne peut pas être changer facilement avec un define. Pour éviter les overflows du buffer RTT, il faut soit réduire le nombre d'événements à suivre ou alors ralentir la work queue pour qu'elle soit plus lente.

---

Dans le dossier `common` se trouve le code qui est utilisé à la fois pour les projets zéphyr et bareMetal, en l'occurrence System View et le measures logger pour la carte SD.

---

### Zéphyr
#### Applications
Dans le dossier Zéphyr se trouve les différentes applications développées pour tester Zéphyr:
- `boards` contient la définition des cartes Nxg_court et Nxg_long
- `fuota` contient le projet qui permet de faire du fuota sur Nxg_long, il doit être compilé avec la commande `west build -p always -b nxg_long --sysbuild fuota -- -DBOARD_ROOT=$PWD` pour compiler en même temps le boot loader puis le flash ensuite avec `west flash`. Pour plus d'informations regarder la section [Fuota](#fuota)
- `custom_board` qui était là pour tester les buttons, les leds, le pwm ainsi que les interruptions sur Nxg_court et ra2l1 (Renesas)
- `i2c` utilise le capteur de température et de luminosité sur le bus i2c de la carte d'évaluation samd21
- `renesas_sd_card` est l'application utilisée pour montrer le bug d'écriture de la carte sd sur la carte renesas ra2l1. Il y a une issue github https://github.com/zephyrproject-rtos/zephyr/issues/90935 sur Zéphyr qui redirige vers ce repo git.
- `zephyr_nxg_court` est l'équivalent du POC en bareMetal avec le capteur de température, de luminosité et la carte SD. En plus, il y a des fonctions pour tester le lora et la rtc. Cette application marche sur nxg_court. La rtc a une erreur à l'initialisation, donc elle ne marche pas. J'ai ouvert un ticket sur microchip, mais pas eu de réponse depuis. La base consommation n'est pas encore développée pour cette carte.
- `led_thread` permet de jouer avec la notion de thread sur Zéphyr ainsi que les priorités

#### Script
Les script `env.sh` et `env.cmd` permettent de définir la variable contenant le chemin vers le repo git zéphyr ainsi que de lancer l'environnement virtuel python, qui permet d'utiliser la commande `west` (commande pour compiler et flasher de Zéphyr).

Le script `path_init_spi_sam0.cmd` permet de supprimer le mot clés `static` de la fonction d'initialisation du bus spi, ainsi pour réduire la consommation de la carte samd21, le bus SPI peut être initialiser et deinitialiser durant l'exécution du code.

#### Fuota
Pour plus d’informations sur le FUOTA, lire la documentation : [Zephyr_documentation.docx](Zephyr_documentation.docx).  
Pour tester le FUOTA, j’ai utilisé RedWoodComm.

- Pour envoyer des binaires d’environ 200 Kb, RedWoodComm possède un bug et envoie parfois des fichiers vides contenant uniquement des 0x00 ou 0xFF. Il faut alors resélectionner le fichier à envoyer ou redémarrer le logiciel pour corriger le problème.
- Une fois tout le binaire reçu, la carte va redémarrer et commencer à échanger les deux binaires. Si le nouveau binaire n’appelle pas la fonction permettant de le rendre permanent, au prochain démarrage il sera remplacé par l’ancien binaire. Si la carte est redémarrée pendant que les binaires sont en cours d’échange, cela ne pose pas de problème : l’algorithme reprendra là où il s’était arrêté.
- Si RedWoodComm estime que la session de FUOTA (classe C) doit durer plus de 9 heures, alors la section définissant la durée de la session va déborder (overflow), et il sera indiqué que la session ne dure qu’une seconde à la place. RedWoodComm n’affiche aucune erreur, mais la session de FUOTA s’arrête instantanément. Il faut donc choisir un SF plus rapide pour que la session dure moins longtemps, ou augmenter la taille des fragments envoyés et, en dernier recours, faire plusieurs sessions de FUOTA à la suite. Zephyr ne supporte pas le fait d’effectuer plusieurs sessions consécutives, car il supprime toute la partition de stockage (celle utilisée pour stocker le nouveau binaire transféré) au début de chaque nouvelle session de FUOTA.
- Zephyr ne supporte pas pour l’instant l’utilisation de la classe B pour faire du FUOTA ou du LoRaWAN tout court.
- Sur RedWoodComm, il n’est pas possible d’entrer manuellement la `redundancy` ou le `packet lost`, il faut forcément utiliser les configurations du menu déroulant, sinon la précédente configuration sera utilisée à la place.
- Sur RedWoodComm, cela ne sert à rien de changer la configuration tant qu’une session de FUOTA est en cours, en prévision d’une prochaine session. En effet, au moment d’arrêter la session en cours (ou quand elle s’arrête), les configurations modifiées sont réinitialisées.
- Avec Zéphyr, dès que les fragments sont reçus, ils sont directement écrits en mémoire dans la partition dédié, sauf pour les fragments de redondance, qui eux sont stockés en RAM jusqu'à ce que le binaire puisse être reconstruit. S'il n'y a pas assez de mémoire en RAM allouer pour ça la session de fuota ça s'arrêter sans dire d'erreur, mais le binaire sera impossible à reconstruire, car il manquera des fragments.
- Avec Zephyr le fichier à envoyer est le fichier `zephyr.signed.bin` car il est hashé et encrypté avec la bonne clé (celle utilisé dans le bootloader), sinon le nouveau binaire ne sera pas considérer comme valide.

### System View
Sur les cartes renesas, connecter la sonde de debug a un produit qui est entrain de dormir va le reveiller 