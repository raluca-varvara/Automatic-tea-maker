# Automatic-tea-maker
Automatic tea maker using arduino

Aparatul contine un motor servo atasat de un brat care introduce pliculetul de ceai in cana, si il ridica atunci cand timpul de infuzare pentru ceaiul ales a expirat. Totul este implementat folosind TimerOne pentru a eficientiza programul folosind intreruperile interne, totul executandu-se pe aceastea, inclusiv miscarea servomotorului, asteptarea infuzarii si citirea senzorului de temperatura care indica momentul cand ceaiul are temperatura optima pentru a fi baut.
Aparatul trece prin niste stari, si este implementat ca un automat cu stari finite, fiecare stare fiind codificata folosind niste variabile boolene cu nume intuitive pentru descifrarea starilor. 
Starile sunt: asteparea alegerii tipului de ceai din butoane (exista 3 butoane, unul pentru ceai negru, unul pentru verde si unul pentru infuzie de fructe), starea de introducere a pliculetului in cana, starea de asteptare, starea de scoatere a pliculetului din cana, si starea de asteptare a racirii ceaiului. 
Toate stagiile vor fi indicate pe un lcd care este conectat la Arduino prin interfata de comunicare I2C. 
