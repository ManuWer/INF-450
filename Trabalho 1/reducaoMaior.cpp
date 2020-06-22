int maiorIndex(int a[], int n) {
	int maior = 0, ind = 0;
	
	for(int i=0;i<n;i++) {
		if(a[i]>maior) {
			maior = a[i];
			ind = i;
		}
	}

	return ind;
}
