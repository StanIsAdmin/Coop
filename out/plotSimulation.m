function plotSimulation(fileName)
  #Load the provided data file
  load([fileName]);
  
  #Intelligence
  plot(avg_intelligence)
  title("Average intelligence per generation")
  xlabel("Simulation time (generations)")
  ylabel("Average intelligence (inner nodes)")
  print -djpg "Intelligence.jpg"
  close
  
  #Cooperation
  plot(avg_cooperation)
  title("Average cooperation frequency per generation")
  xlabel("Simulation time (generations)")
  ylabel("Cooperation frequency")
  print -djpg "Cooperation.jpg"
  close
  
  #Strategies
  stratSum = [count_defect (count_tittat + count_twotat) count_pavlov count_cooper];
  area(stratSum)
  title("Repartiton of pure strategies per generation")
  xlabel("Simulation time (generations)")
  ylabel("Strategies amongst the population")
  legend("Always defect", "Tit for tat-like", "Pavlov-like", "Always cooperate", "location", "southoutside", "orientation", "horizontal")
  legend boxoff
  print -djpg "Strategies.jpg"
  close
  
  #Correlation between cooperation and intelligence
  spearman_cooperation_intelligence = spearman(avg_intelligence, avg_cooperation)