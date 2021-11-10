


n = 8
d = 4


N = 2*n

J = [1,2,3,4]
C = Combinations(range(n),d)
C = list(C)

WK = 2^(n-4)*(binomial(n-4,d)*256 + binomial(n-4,d-1)*240+ binomial(n-4,d-2)*176+ binomial(n-4,d-3)*80+binomial(n-4,d-4)*16)
print('Estimated number of weak keys for equation 20', WK)


WK = 0

for i in range(2^N):
   j = ZZ(i)
   K = Integer (j). digits ( base =2 , padto = N )
   

   for l in range(len(C)):
       I = C[l]
       counter = 1
       for u in range(N/2):

          if(((u in I)==True) and ((u in J)==False) and ((K[u]!=K[N/2+u]) )   ):
              counter = 0
              break

          if(((u in I)==False) and ((u in J)==False) and (K[u]==K[N/2+u]) ):
              counter = 0
              break

          if(((u in I)==True) and ((u in J)==True) and (K[u]==K[N/2+u]) ):
              counter = 0
              break

          
       if(counter==1):
          WK = WK + 1
          break

       
       counter = 1
       for u in range(N/2):

          if(((u in I)==True) and ((u in J)==False) and ((K[u]!=K[N/2+u]) )   ):
              counter =0 
              break

          if(((u in I)==False) and ((u in J)==False) and (K[u]==K[N/2+u]) ):
              counter = 0
              break

          if(((u in I)==True) and ((u in J)==True)  and (K[u]==K[N/2+u]) ):
              counter = 0
              break

          
       if(counter==1):
          WK = WK + 1
          break
       



print('Actual number of weak keys for equation 20', WK) 



print(" ")




sWK = binomial(n-4,d)*2^(n-4-d)*256 + binomial(n-4,d-1)*2^(n-4-d+1)*175+ binomial(n-4,d-2)*2^(n-4-d+2)*67+ binomial(n-4,d-3)*2^(n-4-d+3)*13+binomial(n-4,d-4)*2^(n-4-d+4)


sWK = 2*sWK 


print('Estimated number of weak keys for equation 23', sWK)






sWK = 0
for i in range(2^N):
   j = ZZ(i)
   K = Integer (j). digits ( base =2 , padto = N )
   counter = 0
   for l in range(len(C)):
       I = C[l]
       counter = 1
       for u in range(N/2):

          if(((u in I)==True) and ((u in J)==False) and ((K[u]!=K[N/2+u]) or (K[u]!=0))   ):
              counter = 0
              break

          if(((u in I)==False) and ((u in J)==False) and (K[u]==K[N/2+u]) ):
              counter = 0
              break

          if(((u in I)==True) and ((u in J)==True) and ((K[u]!=0) or (K[N/2+u]!=1)) ):
              counter = 0
              break

          
       if(counter==1):
          sWK = sWK+1
          break
       counter = 1
       for u in range(N/2):

          if(((u in I)==True) and ((u in J)==False) and ((K[u]!=K[N/2+u]) or (K[u]!=1))   ):
              counter = 0
              break

          if(((u in I)==False) and ((u in J)==False) and (K[u]==K[N/2+u]) ):
              counter = 0
              break

          if(((u in I)==True) and ((u in J)==True) and ((K[u]!=1) or (K[N/2+u]!=0)) ):
              counter = 0
              break

          
       if(counter==1):
          sWK = sWK+1
          break
       



print('Actual number of weak keys for equation 23', sWK)











