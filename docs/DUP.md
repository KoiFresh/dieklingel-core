# DoorUnitProtocol

Das DoorUnitProtovcol V1 (Version 1) dient zum Austausch zwischen dem dieklingel-core und weitern Komppnenten. Bei dem Protocol handelt es sich um eine WebSocket (https://de.wikipedia.org/wiki/WebSocket) verbindung, wobei die Basis den WebSocketServer bereitstellt. 
Bei DUP werden Daten in Form des JSON Formats übertragen. Das JSON Objekt muss bestimmte Schlüssel beinhalten, um richtig ausgewertet werden zu können. 

Ein Grundaufbau des Objekts sieht wie folgt aus. 

```javascript
{
  "header":{},
  "body":{
    "context":"log",
    "data":{
    }
  }
}
```

Um nun Informationen zwischen der Basis und weiteren Komponenten auszutauschen, wird dieses Protokoll mit Informationen gefüllt.
Im Key "context" wird der Kontext angegeben, so wird die Aktion festgeltegt, die auf das Protokoll folgt. Es stehen folgende Kontexte zur verfügung:

##### response
Dieser Kontext wird von der Basis benutzt um dem Ct-Gateway eine Rückmeldung zu geben. Falls keine Informationen für die vorherige anfrage Notwendig sind, so ist "data" nicht befüllt. Folgt der Response auf eine Register-Anfrage, so ist "data" mit den Benutzern gefüllt.

##### device-update
Dieser Kontext wird benutzt um Informationen zu einem Bereits Registrierten Gerät zu änderen oder hinzuzufügen. 
Hier sollte "data" mit den Geäte Informationen gefüllt sein. 

Das Protokoll kann wie folgt aussehen:

```javascript
{
  "header":{},
  "body":{
    "context":"device-update
    "data":{
      "devicename":"smd8937q789aind79eadpa",
      "os":"iOS",
      "server":"apple.com",
      //"server":"Android"
      "sound":"normal_sound",
      "token":"dasdjanshuqd23daw78q8287er63qua7dez3ad8r7",
      "username":"sip-username"
    }
  }
}
```
##### connection-state

Diser Kontext ist in DUP V1 (Version 1) nicht belegt.

##### movement

Dieser Kontext wird benutzt um mitzuteilen, dass Bewegung erkannt wurde. Es werden keine Weiteren Informationen benötigt.

##### display-state

Dieser Kontext wird benutzt um mitzuteilen, dass Sich der Zustand der Anzeige/des Displays geändert hat bzw. sich änderen soll. 
Hierbei muss mitgeteilt werden, wie sich der Zustand ändert. Im Bereich "data" wird ein Schlüssel "state" angelegt, welcher die Werte "on" oder "off" annehmen kann.

##### log

Dieser Kontext wird für Logging oder Debugging benutzt. Er enthält einen Schlüssel "string" im bereich "data". Dieser enthält die Log Informationen als Text. 

```javascript
...
    "data":{
      "string":"'20:02:54'[backend.cpp iterate()](info): movement detected"
    }
...
```

##### unlock

Dieser Kontext wird verwendet um mitzuteilen, dass Die Haustür aufgeschlossen wurde, bzw. das Event dafür ausgelöst wurde. 

##### enter-passcode

Dieser Kontext wird verwendet um den Passcode an die Basis zu senden. Woraufhin das unlock event ausgelöst wird.
Das unlock event sendet den Kontext unlock und führt die [on-unlock.sh](https://dieklingel.com/software/?page=config#onunlocksh) Datei aus.

##### call

Dieser Kontext wird verwendet, um der Basis mitzuteilen, das ein Klingelschild gedrückt wurde, woraufhin die Datei [on-ring.sh](https://dieklingel.com/software/?page=config#onringsh) ausgeführt wird.
Es muss folgende Informationen im Bereich "data" bereitstellen: 
```javascript
...
    "data":{
      "sip":"sip-username"
    }
...
```

##### register

Dieser Kontext wird verwendet, um ein neues Gerät Bei der Basis zu Registrieren. Auf diese Nachricht folgt immmer ein "response"-Kontext der Basis mit Benutzerinformationen. Er benötigt keine weitern Informationen im Bereich "data".

##### script

Diser Kontext wird für benutzerdefinierte System Dateie verwendet. Es können weiter on-{eventname}.sh Dateien wie hier(https://dieklingel.com/software/?page=config#onringsh)
beschrieben, angelegt werden. 
Der Kontext script, fürt das aufgeführte in event aufgeführte event/script aus.

```javascript
...
    "data":{
      "event":"light"
    }
...
```

So würde beispielweise das event "light" ausgelöst werden. Die Datei "on-light.sh" wird ausgeführt.

Systemevents, welche Standardmäßig existieren sind: 
```bash
on-ring.sh        => wird beim Läuten aufgerufen
on-unlock.sh      => wird beim Aufschließen aufgerufen
on-call-start.sh  => wird beim starten eines Anrufs aufgerufen
on-call-end.sh    => wird beim beenden eines Anrufs aufgerufen
````
##### unknown

Wird verwendet, wenn kein bekannter Context verwendet wurde.
