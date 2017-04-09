function plotSimulation(fileName)
  #Load the provided data file
  load([fileName]);
  
  #Intelligence
  plot(avg_intelligence)
  title("Average intelligence per generation")
  xlabel("Simulation time (generations)")
  ylabel("Average intelligence (inner nodes)")
  print -djpg "Intelligence.jpg"
  
  #Cooperation
  plot(avg_cooperation)
  title("Average cooperation frequency per generation")
  xlabel("Simulation time (generations)")
  ylabel("Cooperation frequency")
  print -djpg "Cooperation.jpg"
  
  #Correlation between cooperation and intelligence
  spearman_cooperation_intelligence = spearman(avg_intelligence, avg_cooperation)