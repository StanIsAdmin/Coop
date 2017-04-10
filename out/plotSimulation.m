function plotSimulation(fileName)
  #Load the provided data file
  load([fileName]);
  
  graphics_toolkit gnuplot
  
  avg_intelligence = mean(pop_intelligence, 2); #average of rows (each row = one generation)
  
  #Intelligence
  figure('visible','off');
  plot(avg_intelligence)
  title("Average intelligence per generation")
  xlabel("Simulation time (generations)")
  ylabel("Average intelligence (inner nodes)")
  print -djpg "Intelligence.jpg" "-S1920,1080"
  close
  
  #Cooperation
  figure('visible','off');
  plot(avg_cooperation)
  title("Average cooperation frequency per generation")
  xlabel("Simulation time (generations)")
  ylabel("Cooperation frequency")
  print -djpg "Cooperation.jpg" "-S1920,1080"
  close
  
  #Strategies
  figure('visible','off');
  area([count_defect (count_tittat + count_twotat) count_pavlov count_cooper])
  title("Repartiton of pure strategies per generation")
  xlabel("Simulation time (generations)")
  ylabel("Strategies amongst the population")
  legend("Always defect", "Tit for tat-like", "Pavlov-like", "Always cooperate", "location", "southoutside", "orientation", "horizontal")
  legend boxoff
  print -djpg "Strategies.jpg" "-S1920,1080"
  close
  
  #Correlation between cooperation and intelligence
  spearman_cooperation_intelligence = spearman(avg_intelligence, avg_cooperation)