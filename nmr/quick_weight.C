{

  Double_t a=30.9;
  Double_t da=4.5;
  Double_t b=27.1;
  Double_t db=TMath::Sqrt(6.2*6.2+0.96*0.96);
  Double_t c=25;
  Double_t dc=TMath::Sqrt(23*23+86*86);

  Double_t totw=1/(da*da)+1/(db*db)+1/(dc*dc);
  Double_t avg=a/(da*da)+b/(db*db)+c/(dc*dc);
  avg=avg/totw;

  cout << avg << " +- " << TMath::Sqrt(1/totw) << endl;

  Double_t d=2.13;
  Double_t dd=0.29;
  Double_t e=2.05;
  Double_t de=0.78;

  totw=1/(dd*dd)+1/(de*de);
  avg=d/(dd*dd)+e/(de*de);
  avg=avg/totw;

  cout << avg << " +- " << TMath::Sqrt(1/totw) << endl;

}
