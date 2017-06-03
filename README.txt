VALIDATEUR DE BUT V1.0.0

Fonctionnalité présentée : discussion du validateur de but avec les robots

Le dossier contient deux fichiers : socketVB.c à installer sur les PC qui gèrent les buts et socketRobot à installer sur les robots.
Actuellement le code fonctionne de cette manière : 
en attendant de pouvoir traiter les données que nous récupérons de Beebotte, nous créons nous-même  une liste de joueurs. 
De même, en atendant de maitriser le principe de la carte rfid reconnue par arduino, nous renseignons un numero rfid.
Nous comparons alors ce numero rfid à ceux de la liste afin de vérifier si le joueur existe.
Nous récupérons ensuite le numero du robot et la couleur qui lui est associé. 
Nous envoyons alors par socket une demande au robot afin de récuperer son adresseip, la ref du ballon qu'il transporte, et sa couleur.
Nous comparons l'adresse ip reçue et celle qui était enregistrée. Si elle correspondent nous regarderons si le robot vient de marquer dans les bons enbuts, sinon nous mettons fin à la connexion.
Enfin apres avoir verifié que le bon joueur et son robot tiennent un ballon et marquent au bon endroit, nous envoyons ces données au distributeur de ballon afin de verifier que le ballon a bien été attribué à ce joueur.
Dans ce cas, nous signalons au robot qu'il peut retourner jouer apres avoir supprimé son ballon, et mettons fin à la connexion. Nous notifions ensuite sur beebotte le but marqué.
Dans l'autre cas, nous pénalisons le robot et mettons fin à la connexion.
