DELETE FROM `event_scripts` WHERE `id` = 2609 AND `command` = 10;
DELETE FROM `event_scripts` WHERE `id` = 2609 AND `delay` = 600 AND `command` = 11;

INSERT INTO `script_texts` (`entry`, `content_default`, `content_loc3`) VALUES ('-1209000', 'Oh no! Here they come!', 'Oh nein! Hier kommen sie!');
INSERT INTO `script_texts` (`entry`, `content_default`, `content_loc3`) VALUES ('-1209001', 'I\'m out of here!', 'Ich hau ab!'); 
INSERT INTO `script_texts` (`entry`, `content_default`, `content_loc3`, `comment`) VALUES ('-1209002', 'What? How dare you say that to me?!?', 'Was? Ihr wagt es, so mit mir zu sprechen?', 'bly SAY LINE1'); 
INSERT INTO `script_texts` (`entry`, `content_default`, `content_loc3`, `comment`) VALUES ('-1209003', 'After all we\'ve been through? Well, I didn\'t like you anyway!!', 'Nach allem, was wir zusammen durchgemacht haben? Tja, ich konnte Euch eh noch nie leiden!', 'bly SAY LINE2'); 

INSERT INTO `gossip_texts` (`entry`, `content_default`, `content_loc3`) VALUES ('-3209000', 'Please, open the door.', 'Bitte öffne uns das Tor.'); 
INSERT INTO `gossip_texts` (`entry`, `content_default`, `content_loc3`) VALUES ('-3209001', 'That\'s it! I\'m tired of helping you out. It\'s time we settled things on the battlefield!', 'Es reicht! Wir möchten unseren Anteil!'); 
