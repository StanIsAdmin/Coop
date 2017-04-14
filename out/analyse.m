function analyse(filename_base, file_count)
  #Filename details
  [file_path, file_name, file_extension] = fileparts(filename_base)
  
  #GNUplot toolkit is better toolkit
  graphics_toolkit gnuplot
  
  all_pop_intelligence = [];
  all_pop_fitness = [];
  all_cooperation_freq = [];
  all_strategies_count = [];
  
  #Load simulation data and append it to all-simulation data
  for file_index = 1:file_count
    load([file_name, int2str(file_index), file_extension]);
    all_pop_intelligence = [all_pop_intelligence; pop_intelligence];
    all_pop_fitness = [all_pop_fitness; pop_fitness];
    all_cooperation_freq = [all_cooperation_freq; cooperation_freq];
    all_strategies_count = [all_strategies_count; strategies_count];
    
    #Plot individual simulation
    plotSimulation([file_name, int2str(file_index)], pop_intelligence, pop_fitness, cooperation_freq, strategies_count);
  end
  
  plotAverages([file_name int2str(1) "-" int2str(file_count)], all_pop_intelligence, all_pop_fitness, all_cooperation_freq, all_strategies_count);



function plotAverages(file_name, pop_intelligence, pop_fitness, cooperation_freq, strategies_count)
  #Cooperation per mean intelligence
  intelligence_level_precision = 2;
  intelligence_level_max = 10
  intelligence_level_categories = intelligence_level_max*intelligence_level_precision + 1;
  
  avg_intelligence = mean(pop_intelligence, 2);
  
  level_sum_coop = zeros(1, intelligence_level_categories);
  level_count_coop = zeros(1, intelligence_level_categories);
  level_min_coop = zeros(1, intelligence_level_categories);
  level_max_coop = zeros(1, intelligence_level_categories);
  
  for index = 1:size(cooperation_freq)(1)
    intelligence_level = round(avg_intelligence(index)*intelligence_level_precision) + 1;
    cooperation_level = cooperation_freq(index);
    level_sum_coop(intelligence_level) += cooperation_level;
    level_count_coop(intelligence_level) += 1;
    level_min_coop(intelligence_level) = min([level_min_coop(intelligence_level) cooperation_level]);
    level_max_coop(intelligence_level) = max([level_max_coop(intelligence_level) cooperation_level]);
  end
  
  level_x_axis = [0:1.0/intelligence_level_precision:intelligence_level_max];
  level_avg_coop = level_sum_coop ./ level_count_coop;
  level_neg_coop = level_avg_coop .- level_min_coop;
  level_pos_coop = level_max_coop .- level_avg_coop;
  
  figure('visible','off');
  errorbar(level_x_axis, level_avg_coop, level_neg_coop, level_pos_coop, 'o')
  title("Average cooperation per intelligence level")
  ylabel("Frequency of cooperation (min, avg, max)")
  xlabel("Mean level of intelligence")
  xlim([0 intelligence_level_max])
  print([file_name, " Cooperation per Intelligence.png"], "-dpng", "-r600")
  close
    
    


function plotSimulation(file_name, pop_intelligence, pop_fitness, cooperation_freq, strategies_count)
  #Intelligence per generation
  avg_intelligence = mean(pop_intelligence, 2); #average of rows (each row = one generation)
  
  figure('visible','off');
  plot(avg_intelligence)
  title("Average intelligence per generation")
  xlabel("Simulation time (generations)")
  ylabel("Average intelligence (inner nodes)")
  xlim([1 inf])
  print([file_name, " Intelligence.png"], "-dpng", "-r600")
  close
  
  #Cooperation per generation  
  figure('visible','off');
  plot(cooperation_freq)
  title("Average cooperation frequency per generation")
  xlabel("Simulation time (generations)")
  ylabel("Cooperation frequency")
  xlim([1 inf])
  print([file_name, " Cooperation.png"], "-dpng", "-r600")
  close
  
  #Selection for intelligence per generation
  generations = size(pop_intelligence)(1)
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
  print([file_name, " Selection.png"], "-dpng", "-r600")
  close
  
  #Strategies per generation
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
  legend("Defect", "Tit for tat", "Pavlov", "Cooperate", "location", "southoutside", "orientation", "horizontal")
  legend boxoff
  xlim([1 inf])
  print([file_name, " Strategies.png"], "-dpng", "-r600")
  close
  
  #Correlation between cooperation and intelligence
  spearman_cooperation_intelligence = spearman(avg_intelligence, cooperation_freq)
  