


x = load("results2.txt");
hit_percentage = x(1,:);
reaction_time = abs(x(5,:));
orientation = x(6,:);
understanding = x(7,:);
concentration = x(8,:);
memory = x(9,:);



P = [hit_percentage;reaction_time;orientation;understanding;concentration;memory]

radarplot(P,{'Hit Percentage';'Reaction Time'; 'Orientation'; 'Understanding'; 'Concentration'; 'Memory'},'c')
