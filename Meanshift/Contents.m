%% DESCRIZIONE Segmentation by Mean_Shift

% FILES:

    %% distanceCalculator : 
    %        - parametri:
    %               - ingresso: 
    %                       data1 - matrice contenente la media corrente
    %                       data2 - matrice dei punti
    %               - uscita: d - matrice delle distance (spaziali e
    %                              cromatiche)
    %
    %        - descrizione: questa funzione effettua il calcolo delle
    %          distanze spaziali e cromatiche tra la media corrente e tutti
    %          i punti dell'insieme dati in accordo alle distanze e quindi 
    %          al calcolo delle distanze Euclidee. Come output dar‡ una 
    %          matrice in cui nella prima riga si avranno tutte le distanze
    %          spaziali, nella seconda riga tutte le distanze cromatiche.
    %          
    %         - N.B. : Utilizzando la vettorizzazione e le operazioni
    %           matriciali di MatLab, Ë stato possibile effettuare il calcolo
    %           delle distanze per ogni punto in sole 2 righe di codice
    %           senza utilizzare cicli for, migliorando in tal modo
    %           sensibilmente l'efficienza del codice.
    
    
    %% meanShift : 
    %        - parametri:
    %               - ingresso: 
    %                       data     - dati da segmentare, (num_dim x num_pts)
    %                       H        - distanza spaziale e cromatica, [hs ; hr]
    %                       distance - funzione che calcola la distanza tra la media attuale e
    %                                  gli altri punti
    %
    %               - uscita: 
    %                       centers_Of_Clusters      - posizione del centro di ogni cluster
    %                       data_cluster             - indici dei cluster (associa ogni punto al
    %                                                  cluster d'appartenenza)
    %
    %        - descrizione: Effettua la segmentazione dei dati passati come
    %          input. 
    %          Si descriver‡ (sommariamente) l'operato della funzione in
    %          quattro grandi fasi (per maggiore dettaglio si rimanda ai
    %          commenti del codice):
    %
    %          I FASE:  
    %          Viene calcolata la dimensione dei dati, impostato un valore 
    %          soglia (che servir‡ a determinare la conclusione del
    %          processo di mean shift) e vengono create una serie di
    %          variabili e di strutture dati necessarie per l'esecuzione
    %          dell'algoritmo.
    %
    %          II FASE:
    %          Viene scelto, in maniera random, un punto da cui far partire
    %          l'algoritmo. FinchË la norma della differenza tra la nuova 
    %          media e quella precedente Ë sopra la soglia: 1)viene calcolata
    %          la distanza(spaziale e cromatica) tra la media corrente e
    %          tutti i punti; 2)vengono considerati solo i punti che sono 
    %          all'interno del range spaziale e cromatico; 3) viene
    %          calcolata la nuova media ed aggiornati i voti per il cluster
    %          corrente.
    %          
    %          III FASE:
    %          Si verifica se il nuovo cluster coincide con uno di quelli gi‡ trovati
    %          (ossia se la media finale Ë vicina ad uno degli elementi di
    %          'centers_Of_Clusters', cioË ad un altro picco gi‡ trovato), 
    %          se vi Ë coincidenza, si aggiorna il centro con la media tra
    %          i due; se non vi Ë coincidenza, verr‡ aggiunto un nuovo
    %          cluster. Si aggiorna poi il conteggio dei punti che rimangono da
    %          visitare
    %           
    %          IV FASE:
    %          Vengono scelti gli indici dei cluster e si associa ogni punto
    %          al cluster al quale ha dato pi˘ voti.
    %
    %
    %         - N.B. : Attraverso l'uso della
    %           funzione find, Ë stata possibile la soppressione di cicli
    %           for. 
    %           Nell'algoritmo, Ë stato previsto l'uso dello 'speed up'
    %           "bacino di attrazione" come
    %           previsto da Comaniciu & Meer.
    
 
    
    %% segmentation : 
    %        - parametri:
    %               - ingresso: 
    %                       Image    - immagine da segmentare
    %                       H        - distanza spaziale e cromatica, [hs ; hr]
    %                       distance - funzione che calcola la distanza tra la media attuale e
    %                                  gli altri punti
    %
    %               - uscita:
    %                       segmented_Image          - Immagine segmentata
    %                       centers_Of_Clusters      - posizione del centro di ogni cluster
    %                       point_To_Cluster         - indici dei cluster (associa ogni punto al
    %                                                  cluster d'appartenenza)
    %
    %        - descrizione: Questa funzione prende in input l'immagine da
    %          segmentare ed altri due parametri che serviranno per
    %          invocare la funzione 'meanShift'. L'operato di questa funzione 
    %          puÚ essere riassunta in tre grandi fasi:
    %
    %          I FASE: 
    %          Attraverso operazioni di reshape e vettorizzazione, l'immagine 
    %          viene posta in una struttura dati utile per il mean shift;
    %
    %          II FASE:
    %          Viene invocata la funzione main shift che effettuer‡ la segmentazione;
    %
    %          III FASE:
    %          I dati restituiti dalla funzione mean shift vengono portati
    %          nel formato dell'immagine originale, viene poi creata
    %          l'immagine segmentata sostituendo ad ogni punto il colore del suo
    %          cluster d'appartenenza
    %          
    %         - N.B. : L'utilizzo della funzione MatLab 'find' nella
    %           16esima riga di codice, ha permesso l'eliminazione di cicli for   
    %           altrimenti necessari per ottenere le componenti spaziali
    %           dell'immagine.
    
    
    %% main : 
    %        - parametri:
    %               - ingresso: nessuno
    %               - uscita: nessuno
    %
    %        - descrizione: Questa Ë una funzione di utilit‡, che 
    %          consente di effettuare la segmentazione di un'immagine
    %          avendo dei parametri di prova gi‡ caricati, senza l'immissione 
    %          di alcun valore da parte dell'utilizzatore; baster‡
    %          semplicemente invocare la funzione.
    %
    %        - N.B. : % Come esempio di utilizzo, alla funzione 'segmentation',
    %          invocata nel main, verranno passati i paramentri:
    %          [8 ; 16] rispettivamente hs e hr.
    %          La scelta di hs ed hr èË stata dettata da un evidente convenienza emersa
    %          nella fase di sperimentazione; difatti, a fronte di un tempo di
    %          computazione accettabile, questi parametri consentono che al termine
    %          della segmentazione siano presenti regioni con un ragionevole contrasto
    %          di colore ed un supporto spaziale abbastanza ampio.
    
    
    
    %% RIFERIMENTI:
    %
    %         - D. Comaniciu and P. Meer. Mean shift: A robust approach toward feature space
    %           analysis. In IEEE Transactions on Pattern Analysis and Machine Intelligence,
    %           2002.
    