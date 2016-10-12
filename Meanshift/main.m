%%File di utilit�, lancia la segmentazione con Mean Shift utilizzando parametri
%%precaricati.

function main()

original_Image=imread('image.jpg');

% Si invoca la funzione 'segmentation'che, utilizzando la funzione mean
% shift, effettua la segmentazione dell'immagine

% Come esempio di utilizzo, alla funzione verranno passati i paramentri:
% [8 ; 16] rispettivamente hs e hr.
% La scelta di hs ed hr � stata dettata da un evidente convenienza emersa
% nella fase di sperimentazione; difatti, a fronte di un tempo di
% computazione accettabile, questi parametri consentono che al termine
% della segmentazione siano presenti regioni con un ragionevole contrasto
% di colore ed un abbastanza ampio supporto spaziale.

[clusters, pointClusters, imm]=segmentation(original_Image,[8;16],@distanceCalculator);

figure, imshow(original_Image);

figure, imshow(imm);



