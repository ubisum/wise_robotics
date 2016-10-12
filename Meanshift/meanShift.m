%%Realizza una segmentazione tramite mean shift con kernel uniforme

function [centers_Of_Clusters ,data_cluster] = meanShift(data, H,distance)


% ---INPUT---
% data              - dati da segmentare, (num_dim x num_pts)
% H                 - distanza spaziale e cromatica, [hs ; hr]
% distance          - funzione che calcola la distanza tra la media attuale e
%                     gli altri punti

% ---OUTPUT---
% centers_Of_Clusters      - posizione del centro di gni cluster
% data_cluster             - indici dei cluster (associa ogni punto al
%                            cluster d'appartenenza)


%soglia di convergenza del mean shift
stop_threshold = 1e-2*(prod(H)^(1/length(H)));

%calcola la dimensione dei dati 
[dimension_Number, pointsNumber] = size(data);
pointsNumber=round(pointsNumber);

%punti ancora non visitati
point_to_visit = 1:pointsNumber;
number_to_visit = length(point_to_visit);

%punti già visitati
visitedPoints = zeros(1, pointsNumber);

%creiamo il vettore dei centri dei vari cluster
centers_Of_Clusters = [];
%centers_Of_Clusters = zeros(5,pointsNumber);
clusters_Number = 0;


cluster_votes = zeros(1, pointsNumber);


while (number_to_visit)
    %si sceglie un punto a caso tra quelli da visitare
    startingPoint = ceil(rand * number_to_visit);
    
    %Inizialmente la media è data dal punto stesso
    current_Mean = data(:,point_to_visit(startingPoint));
    
    previus_Mean = current_Mean + stop_threshold*2;
    
    %indice dei punti che verranno aggiunti al cluster finale
    cluster_members = [];
    
    curr_cluster_votes = zeros(1, pointsNumber);
   
    %finche la norma della differenza tra la nuova media e quella precedente è sopra la
    %soglia
    while (norm(current_Mean - previus_Mean) > stop_threshold)
    
        %si calcola la distanza tra la media attuale e tutti i punti
        dist=distance(repmat(current_Mean,1,pointsNumber),data);
        

        %si considerano solo i punti che sono all'interno del range
        %spaziale e cromatico
        inside_Points_index = find (min(dist < repmat(H,1,pointsNumber)));
        
        previus_Mean = current_Mean;
        
        %si calcola la nuova media 
        current_Mean = mean(data(:,inside_Points_index),2);
        
        %si aggiornano i voti per il cluster corrente
        curr_cluster_votes(inside_Points_index) = curr_cluster_votes(inside_Points_index) + 1;
        
        cluster_members = unique([cluster_members inside_Points_index]);
        
        %si memorizzano i punti già visitati
        visitedPoints(inside_Points_index) = 1;
        
    
    end
     
     %si verifica se il nuovo cluster coincide con uno di quelli già trovati
     %(ossia se la media finale è vicina ad uno degli elementi di
     %'centers_Of_Clusters')
     
     clusters_Are_Merged = 0;
     for i=1:clusters_Number
         distance1 = distance(current_Mean, centers_Of_Clusters(:,i));
         
         if (min(distance1 < H))        
             clusters_Are_Merged = i;
             break;
         end
     end
     
     %se vi è coincidenza, si aggiorna il centro con la media tra i due
     if (clusters_Are_Merged > 0)
         centers_Of_Clusters(:,i) = 0.5*(centers_Of_Clusters(:,i) + current_Mean);
         cluster_votes(i,:) = cluster_votes(i,:) + curr_cluster_votes;
         
     %se non vi è coincidenza, verrà aggiunto un nuovo cluster
     else
         clusters_Number = clusters_Number+1;
         centers_Of_Clusters(:,clusters_Number) = current_Mean;
         cluster_votes(clusters_Number, :) = curr_cluster_votes;
     end
     
     %Si aggiorna il conteggio dei punti che rimangono da visitare
     point_to_visit = find(visitedPoints == 0);
     number_to_visit = length(point_to_visit);

end
 
[val, data_cluster] = max(cluster_votes,[],1);
  


