%% La funzione si occupa del calcolo della distanza fra la media attuale e
%% tutti i punti

function [d] = distanceCalculator(data1,data2)

%Controlliamo che le dimensioni coincidano
if(size(data1)~=size(data2))
    error('Dimension mismatch');
end

%Si prealloca una matrice che conterrà le distanze
d=zeros(2,size(data1,2));

%Calcolo delle distanze spaziali 
d(1,:)= sqrt( sum ((data1(1:2,:) - data2(1:2,:)).^2,1) );

%Calcolo delle distanze cromatiche
d(2,:)= sqrt( sum ((data1(3:size(data1,1),:) - data2(3:size(data2,1),:)).^2,1) ); 

end