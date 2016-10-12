%%Questa funzione effettua la segmentazione di un'immagine, avvalendosi
%%dell'algoritmo mean shift
function [centers_Of_Clusters, point_To_Cluster,segmented_Image]= segmentation(Image,H,distance)

I=double(Image);

%Si crea una struttura dati adatta alle esigenze
data=zeros(size(I,3)+2,size(I,1)*size(I,2));

%Si inseriscono nella matrice le componenti cromatiche della nostra immagine
for i=1:size(I,3)
    data(i+2,:)=reshape(I(:,:,i),1,size(I,1)*size(I,2));
end

%Si ricavano le componenti spaziali dell'immagine
[r c]=find(I(:,:,1)<Inf);

%Si inseriscono matrice le componenti spaziali della nostra immagini
data(1,:)=r';
data(2,:)=c';

%Viene invocata la funzione meanShift per effettuare la segmentazione 
[centers_Of_Clusters,point2c]=meanShift(data,H,distance);

%Dopo aver effettuato la segmentazione, si riporta il risultato nel formato
%dell'immagine originale
point_To_Cluster=reshape(point2c,size(I,1),size(I,2));

segmented_Image=zeros(size(I));

%Creiamo l'immagine in cui ad ogni punto è sostiuito il colore del suo
%cluster d'appartenenza
for i=1:size(I,1)
    for j=1:size(I,2)
        for k=1:size(I,3)
            segmented_Image(i,j,k)=centers_Of_Clusters(2+k,point_To_Cluster(i,j));
        end
    end
end

segmented_Image=uint8(segmented_Image);

end
