function plotSimulation(fileName)
  #Load the provided data file
  load([fileName]);
  
  name_without_extension = strtok(fileName, '.');
  generations = size(pop_intelligence)(1)
  
  graphics_toolkit gnuplot
  
  #Intelligence
  avg_intelligence = mean(pop_intelligence, 2); #average of rows (each row = one generation)
  
  figure('visible','off');
  plot(avg_intelligence)
  title("Average intelligence per generation")
  xlabel("Simulation time (generations)")
  ylabel("Average intelligence (inner nodes)")
  xlim([1 inf])
  print([name_without_extension, " Intelligence.jpg"], "-djpg", "-S1920,1080")
  close
  
  #Cooperation
  avg_cooperation = cooperation_defection(:,1) ./ (cooperation_defection(:,1).+cooperation_defection(:,2));
  
  figure('visible','off');
  plot(avg_cooperation)
  title("Average cooperation frequency per generation")
  xlabel("Simulation time (generations)")
  ylabel("Cooperation frequency")
  xlim([1 inf])
  print([name_without_extension, " Cooperation.jpg"], "-djpg", "-S1920,1080")
  close
  
  #Selection for intelligence
  avg_fitness = mean(pop_fitness, 2);
  selection_for_intelligence = zeros(generations,1);
  for row_index = 1:generations
    selection_for_intelligence(row_index) = cov(pop_intelligence(row_index,:), pop_fitness(row_index,:)) / avg_fitness(row_index);
  end
  figure('visible','off');
  plot(selection_for_intelligence)
  title("Selection for intelligence")
  xlabel("Simulation time (generations)")
  ylabel("Covariance between intelligence and fitness")
  xlim([1 inf])
  print([name_without_extension, " Selection.jpg"], "-djpg", "-S1920,1080")
  close
  
  #Strategies
  count_cooper = strategies_count(:,1);
  count_defect = strategies_count(:,2);
  count_tittat = strategies_count(:,3);
  count_twotat = strategies_count(:,4);
  count_pavlov = strategies_count(:,5);
  
  figure('visible','off');
  area([count_defect (count_tittat + count_twotat) count_pavlov count_cooper])
  title("Repartiton of pure strategies per generation")
  xlabel("Simulation time (generations)")
  ylabel("Strategies amongst the population")
  legend("Always defect", "Tit for tat-like", "Pavlov-like", "Always cooperate", "location", "southoutside", "orientation", "horizontal")
  legend boxoff
  xlim([1 inf])
  print([name_without_extension, " Strategies.jpg"], "-djpg", "-S1920,1080")
  close
  
  #Correlation between cooperation and intelligence
  spearman_cooperation_intelligence = spearman(avg_intelligence, avg_cooperation)
  