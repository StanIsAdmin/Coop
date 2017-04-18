function analyse(file_base, file_count)
  #Filename details
  [file_path, file_name, file_extension] = fileparts(file_base);
  
  #GNUplot toolkit is better toolkit
  graphics_toolkit gnuplot
  
  #All-simulations data
  all_pop_intelligence = [];
  all_pop_fitness = [];
  all_cooperation_freq = [];
  all_strategies_count = [];
  all_selection_for_intelligence = [];
  
  for file_index = 1:file_count
    #Load simulation data and
    load([file_name, int2str(file_index), file_extension]);
    
    #Pool together tit-for-tat and tit-for-two tats strategies
    strategies_count = [strategies_count(:,1:2) (strategies_count(:,3) .+ strategies_count(:,4)) strategies_count(:,5)];
    
    #Append simulation data to all-simulations data
    all_pop_intelligence = [all_pop_intelligence; pop_intelligence];
    all_pop_fitness = [all_pop_fitness; pop_fitness];
    all_cooperation_freq = [all_cooperation_freq; cooperation_freq];
    all_strategies_count = [all_strategies_count; strategies_count];
    
    #Plot individual simulation
    selection_for_intelligence = plotSimulation([file_name, int2str(file_index)], pop_intelligence, pop_fitness, cooperation_freq, strategies_count);
    all_selection_for_intelligence = [all_selection_for_intelligence; selection_for_intelligence];
  end
  
  plotAverages([file_name "0-" int2str(file_count)], all_pop_intelligence, all_pop_fitness, all_cooperation_freq, all_strategies_count, all_selection_for_intelligence);
end


function plotAverages(file_name, pop_intelligence, pop_fitness, cooperation_freq, strategies_count, selection_for_intelligence)
  #Cooperation per mean intelligence
  plotCooperationByIntelligence(file_name, pop_intelligence, cooperation_freq);
  
  #Correlation between strategy frequency and selection for intelligence
  plotStrategiesSelection(file_name, pop_intelligence, pop_fitness, cooperation_freq, strategies_count);
  
  #Selection for intelligence and transitions to cooperation
  plotTransitionsToCooperation(file_name, cooperation_freq, selection_for_intelligence);
  
  #Correlation between cooperation and intelligence
  #spearman_cooperation_intelligence = spearman(avg_intelligence, cooperation_freq)
end


function plotCooperationByIntelligence(file_name, pop_intelligence, cooperation_freq)
  intelligence_level_max = 20;
  intelligence_level_precision = 2;
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
end


function plotStrategiesSelection(file_name, pop_intelligence, pop_fitness, cooperation_freq, strategies_count)
  generations = size(pop_intelligence)(1);
  avg_fitness = mean(pop_fitness, 2);
  avg_intelligence = mean(pop_intelligence, 2);
  
  selection_for_intelligence_all = zeros(generations,1);
  selection_for_intelligence_low = [];
  selection_for_intelligence_high = [];
  
  strategies_count_all = strategies_count;
  strategies_count_low = [];
  strategies_count_high = [];
  
  for row_index = 1:generations
    selection_for_intelligence_all(row_index) = cov(pop_intelligence(row_index,:), pop_fitness(row_index,:)) / avg_fitness(row_index);
    
    if (cooperation_freq(row_index) < 0.5)
      selection_for_intelligence_low = [selection_for_intelligence_low; selection_for_intelligence_all(row_index, :)];
      strategies_count_low = [strategies_count_low ; strategies_count_all(row_index, :)];
    else 
      selection_for_intelligence_high = [selection_for_intelligence_high; selection_for_intelligence_all(row_index, :)];
      strategies_count_high = [strategies_count_high ; strategies_count_all(row_index, :)];
    end 
  end
  
  spearman_selection_strategies = zeros(3, 4);
  for strategy_index = 1:4
    spearman_selection_strategies(1, strategy_index) = spearman(strategies_count_all(:, strategy_index), selection_for_intelligence_all);
    try 
      spearman_selection_strategies(2, strategy_index) = spearman(strategies_count_low(:, strategy_index), selection_for_intelligence_low);
    catch
      spearman_selection_strategies(2, strategy_index) = 0;
    end
    try 
      spearman_selection_strategies(3, strategy_index) = spearman(strategies_count_high(:, strategy_index), selection_for_intelligence_high);
    catch exception
      spearman_selection_strategies(3, strategy_index) = 0;
    end
      
  end
  
  figure('visible','off');
  h = bar(spearman_selection_strategies);
  set(h(1), "facecolor", "k")
  set(h(2), "facecolor", "w")
  set(h(3), "facecolor", [.60 .60 .60])
  set(h(4), "facecolor", [.83 .83 .83])
  series_names = {"all data", "coop < 0.5", "coop >= 0.5"};
  set(gca,'xticklabel', series_names)
  title("Spearman rank correlation between strategies and selection for intelligence")
  ylabel("Spearman rank correlation")
  xlabel("Strategies")
  print([file_name, " Strategies and Selection.png"], "-dpng", "-r600")
  close
end


function plotTransitionsToCooperation(file_name, cooperation_freq, selection_for_intelligence)
  generations = size(cooperation_freq)(1);
  cooperation_average = cooperation_freq(2:generations, :);
  cooperation_change = cooperation_average .- cooperation_freq(1:generations-1,:);

  coop_avg_min = min(cooperation_average);
  coop_avg_max = max(cooperation_average);
  coop_avg_diff = coop_avg_max - coop_avg_min;
  coop_change_min = min(cooperation_change);
  coop_change_max = max(cooperation_change);
  coop_change_diff = coop_change_max - coop_change_min;
    
  precision = 100;
  Z = zeros(precision, precision);
  X = coop_avg_min:(coop_avg_diff/100):coop_avg_max;
  Y = coop_change_min:(coop_change_diff/100):coop_change_max;
  for index = 1:generations-1
    coop_avg_current = cooperation_average(index);
    x_current = round(((coop_avg_current - coop_avg_min) / coop_avg_diff) * precision);
    coop_change_current = cooperation_change(index);
    y_current = round(((coop_change_current - coop_change_min) / coop_change_diff) * precision);
    Z(y_current+1, x_current+1) = selection_for_intelligence(index+1);
  end
  
  figure('visible','off');
  colormap gray
  contourf(X,Y,Z)
  title("Selection for intelligence and transitions to cooperation")
  xlabel("Cooperation frequency")
  ylabel("Change in cooperation")
  print([file_name, " Transitions to Cooperation.png"], "-dpng", "-r600")
  close
end


function selection_for_intelligence = plotSimulation(file_name, pop_intelligence, pop_fitness, cooperation_freq, strategies_count)
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
  generations = size(pop_intelligence)(1);
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
  figure('visible','off');
  h = area(strategies_count);
  title("Repartiton of pure strategies per generation")
  xlabel("Simulation time (generations)")
  ylabel("Strategies amongst the population")
  set(h(1), "facecolor", "k")
  set(h(2), "facecolor", "w")
  set(h(3), "facecolor", [.60 .60 .60])
  set(h(4), "facecolor", [.83 .83 .83])
  xlim([1 inf])
  print([file_name, " Strategies.png"], "-dpng", "-r600")
  close

end
