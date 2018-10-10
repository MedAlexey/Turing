# Turing
### Вид командной строки для запуска программы: 
`turing.exe description.txt tape.txt out.txt -d`

"-d" является необязательным параметром, запускающим режим отладки

### Оформление файла "description.txt":

В данном файле задаётся описание машины

    количество лент
    используемый алфавит
    "пустой символ"
    начальное состояние
    описание состояний
    
    
Пример:

    3
    01
    _
    q1
    q1: _,_,_->_,R;_,R;_,R;->q2;
    q2: 1,_,_->1,R;1,R;1,R;->q2; 0,_,_->0,R;0,R;0,R;->q2; _,_,_->_,L;_,L;_,L;->q3;
    q3: 1,1,1->1,L;0,L;1,L;->q3; 0,0,0->0,L;1,L;0,L;->q3; _,_,_->_,R;_,R;_,R;->q0;
  

### Оформление файла "tape.txt":

В данный файл записываются входные ленты и указываются положения кареток на лентах с помощью символа "v"

Пример:

    tape1:
       v
    _101011_
    tape3:
     v
    01_11 


### 
