
## Modular Exponentiation based on Simultaneous Multi-Exponentiation (Shamir Method)

Analysis complexity of Joint Signed Digit Representations for DSA (Optimize operation: R = gk mod P and R =y r + g s  mod P, P - module):
-	Use Shamir method to decomposite k to n parts (experienced with n from 1 to 5).
-	Convert each part of k to different representation types: multi-dimension binary, joint sparse form (JSF), higher dimensional joint sparse form (d-JSF, d = [2,5]).
-	Compute and minimize the complexity of simultaneous multi-exponentiation algorithm using new converted numbers formats by the number of square and multiply operations. 
-	Optimize the running time of the algorithms (C/C++, library: miracl). 
-	Compare results and conclude effective algorithms.
