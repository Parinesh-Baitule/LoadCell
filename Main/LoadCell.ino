void LoadCell(){
  
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  ActualWeight = scale.get_units();
  //Serial.print("Reading: ");
  
    FinalWeight = ((ActualWeight * LB2GRM)/100);
    Serial.print(FinalWeight, 1);

  
  
  //Serial.print(" lbs"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  //Serial.println(" ");
}

