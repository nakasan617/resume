// in this file I am going to read various file with different size and compare the performance. 
// I probably need to do one kilo byte, ten kilo byte, hundred kilo byte, one mega byte, ten mega byte, and if possible, hundred megabyte.  
// output it as a csv file 

#include "../parse.h"
#include "../merkletree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double time_create_merkle(char path [], int binary) {
	clock_t start = clock();
	FILE * fp = NULL;
	if(binary) {
		fp = fopen(path, "rb");
	} else {
		fp = fopen(path, "r");
	}
	if(!fp) {
		perror("fp is null\n");
		return -1;
	}

        merkle_tree mt;
	if(compute_merkle(&fp, &mt, 1) == -1) {
		perror("error in computing merkle\n");
		return -1;
	}
	clock_t end = clock();
	printf("this is the nb_of_pages %lu\n", mt.nb_of_pages);
	free_merkle(&mt);
	fclose(fp);
	double rv = (double)(end - start)/CLOCKS_PER_SEC;
	printf("it took %lf seconds to make merkle tree for %s\n", rv, path);
	return rv;
}

double * time_portion_create_merkle(char path [], int binary) {
	// times[0], time to parse 
	// times[1], time to build tree
	// times[2], time to hash
	// times[3], time to hash for leaves
	// times[4], time to do the whole iteration 
	double * times = (double *)calloc(5, sizeof(double));
	clock_t start = clock();
	FILE * fp = NULL;
	if (binary) {
		fp = fopen(path, "rb");
	} else {
		fp = fopen(path, "r");
	}

	if(!fp) {
		perror("fp is null\n");
		return NULL;
	}

	merkle_tree mt; 
	if(compute_merkle_time(&fp, &mt, 1, times) == -1) {
		perror("error in computing merkle\n");
		return NULL;
	}
	clock_t end = clock();
	times[4] = (double)(end - start)/CLOCKS_PER_SEC; 
	printf("it took %lf seconds to parse for %s\n", times[0], path);
	printf("it took %lf seconds to build tree for %s\n", times[1], path);
	printf("it took %lf seconds to hash %s\n", times[2], path);
	printf("it took %lf seconds for the whole thing %s\n", times[4], path);

	free_merkle(&mt);
	fclose(fp);
	return times;
}

int text_separate() {
	FILE * fp_ = fopen("separate_create_merkle_txt.csv", "w");
	
	// 1K
	double * times = time_portion_create_merkle("./example/one_kilo.txt", 0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	 
	// 10K 
	times = time_portion_create_merkle("./example/ten_kilo.txt", 0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 100K
	times = time_portion_create_merkle("./example/hundred_kilo.txt",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	
	// 1M
	times = time_portion_create_merkle("./example/one_mega.txt",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 10M 
	times = time_portion_create_merkle("./example/ten_mega.txt",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 100M
	times = time_portion_create_merkle("./example/hundred_mega.txt",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 1G
	times = time_portion_create_merkle("./example/one_giga.txt",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 10G
	times = time_portion_create_merkle("./example/ten_giga.txt",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	

	fclose(fp_);
	return 0;
}

int text_binary_separate() {
	FILE * fp_ = fopen("separate_create_merkle_binary_txt.csv", "w");
	
	// 1K
	double * times = time_portion_create_merkle("./example/one_kilo.txt", 1);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	 
	// 10K 
	times = time_portion_create_merkle("./example/ten_kilo.txt", 1);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 100K
	times = time_portion_create_merkle("./example/hundred_kilo.txt",1);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	
	// 1M
	times = time_portion_create_merkle("./example/one_mega.txt",1);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 10M 
	times = time_portion_create_merkle("./example/ten_mega.txt",1);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 100M
	times = time_portion_create_merkle("./example/hundred_mega.txt",1);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 1G
	times = time_portion_create_merkle("./example/one_giga.txt",1);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 10G
	times = time_portion_create_merkle("./example/ten_giga.txt",1);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	

	fclose(fp_);
	return 0;
}

int pdf_separate() {
	FILE * fp_ = fopen("separate_create_merkle_pdf.csv", "w");
	
	// 1K
	double * times = time_portion_create_merkle("./dummypdfs/1.00KB.pdf",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	 
	// 10K
	times = time_portion_create_merkle("./dummypdfs/10.00KB.pdf",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 100K
	times = time_portion_create_merkle("./dummypdfs/100.00KB.pdf",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 1M
	times = time_portion_create_merkle("./dummypdfs/1.00MB.pdf",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 10M
	times = time_portion_create_merkle("./dummypdfs/10.01MB.pdf",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	fclose(fp_);
	return 0;
}

int docx_separate() {
	FILE * fp_ = fopen("separate_create_merkle_docx.csv", "w");
	
	// 1K
	double * times = time_portion_create_merkle("./dummydocx/one_kilo.docx",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	 
	// 10K
	times = time_portion_create_merkle("./dummydocx/ten_kilo.docx",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 100K
	times = time_portion_create_merkle("./dummydocx/hundred_kilo.docx",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 1M
	times = time_portion_create_merkle("./dummydocx/one_mega.docx",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 10M
	times = time_portion_create_merkle("./dummydocx/ten_mega.docx",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);


	fclose(fp_);
	return 0;
}

int NETCDF_separate() {
	FILE * fp_ = fopen("separate_create_merkle_netcdf.csv", "w");
	
	// 1K
	double * times = time_portion_create_merkle("./NetCDF/one_kilo.nc",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	 
	// 10K
	times = time_portion_create_merkle("./NetCDF/ten_kilo.nc",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 100K
	times = time_portion_create_merkle("./NetCDF/hundred_kilo.nc",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 1M
	times = time_portion_create_merkle("./NetCDF/one_mega.nc",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 10M
	times = time_portion_create_merkle("./NetCDF/ten_mega.nc",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	


	fclose(fp_);
	return 0;
}

int hdf5_separate() {
	FILE * fp_ = fopen("separate_create_merkle_hdf5.csv", "w");
	
	// 1K
	double * times = time_portion_create_merkle("./HDF5/one_kilo.h5",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	 
	// 10K 
	times = time_portion_create_merkle("./HDF5/ten_kilo.h5",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 100K
	times = time_portion_create_merkle("./HDF5/hundred_kilo.h5",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	
	// 1M
	times = time_portion_create_merkle("./HDF5/one_mega.h5",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	
	// 10M 
	times = time_portion_create_merkle("./HDF5/ten_mega.h5",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 100M
	times = time_portion_create_merkle("./HDF5/hundred_mega.h5",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 1G
	times = time_portion_create_merkle("./HDF5/one_giga.h5",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);

	// 10G
	times = time_portion_create_merkle("./HDF5/ten_giga.h5",0);
	if(!times) 
		return -1;
	fprintf(fp_, "%lf:%lf:%lf:%lf:%lf, ", times[0], times[1], times[2], times[3], times[4]);
	free(times);
	

	fclose(fp_);
	return 0;
}


int text() {
	FILE * fp_ = fopen("time_create_merkle_txt.csv", "w");
	double write = 0;
	write = time_create_merkle("./example/one_kilo.txt",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/ten_kilo.txt",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/hundred_kilo.txt",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/one_mega.txt",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/ten_mega.txt",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/hundred_mega.txt",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/one_giga.txt",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/ten_giga.txt",0);	
	fprintf(fp_, "%lf\n", write);
	fclose(fp_);
	
	return 0;
}

int text_binary() {
	FILE * fp_ = fopen("time_create_merkle_txt_binary.csv", "w");
	double write = 0;
	write = time_create_merkle("./example/one_kilo.txt",1);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/ten_kilo.txt",1);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/hundred_kilo.txt",1);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/one_mega.txt",1);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/ten_mega.txt",1);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/hundred_mega.txt",1);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/one_giga.txt",1);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./example/ten_giga.txt",1);	
	fprintf(fp_, "%lf\n", write);
	fclose(fp_);
	
	return 0;
}

int pdf() {
	FILE * fp_ = fopen("time_create_merkle_pdf.csv", "w");
	double write = 0;
	write = time_create_merkle("./dummypdfs/1.00KB.pdf",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./dummypdfs/10.00KB.pdf",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./dummypdfs/100.00KB.pdf",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./dummypdfs/1.00MB.pdf",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./dummypdfs/10.01MB.pdf",0);
	fprintf(fp_, "%lf\n", write);
	fclose(fp_);
	
	return 0;
}

int docx() {
	FILE * fp_ = fopen("time_create_merkle_docx.csv", "w");
	double write = 0;
	write = time_create_merkle("./dummydocx/one_kilo.docx",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./dummydocx/ten_kilo.docx",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./dummydocx/hundred_kilo.docx",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./dummydocx/one_mega.docx",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./dummydocx/ten_mega.docx",0);
	fprintf(fp_, "%lf\n", write);
	fclose(fp_);
	return 0;
}

int NetCDF() {
	FILE * fp_ = fopen("time_create_merkle_NetCDF.csv", "w");
	double write = 0;
	write = time_create_merkle("./NetCDF/one_kilo.nc",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./NetCDF/ten_kilo.nc",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./NetCDF/hundred_kilo.nc",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./NetCDF/one_mega.nc",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./NetCDF/ten_mega.nc",0);
	fprintf(fp_, "%lf\n", write);
	fclose(fp_);
	
	return 0;
}

int h5() {
	FILE * fp_ = fopen("time_create_merkle_h5.csv", "w");
	double write = 0;
	write = time_create_merkle("./HDF5/one_kilo.h5",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./HDF5/ten_kilo.h5",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./HDF5/hundred_kilo.h5",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./HDF5/one_mega.h5",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./HDF5/ten_mega.h5",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./HDF5/hundred_mega.h5",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./HDF5/one_giga.h5",0);
	fprintf(fp_, "%lf, ", write);
	write = time_create_merkle("./HDF5/ten_giga.h5",0);	
	fprintf(fp_, "%lf\n", write);
	fclose(fp_);
	
	return 0;
}

int main() {
	
	text();
	text_binary();
	/*
	pdf(); 
	docx();
	NetCDF();
	h5();
	*/
	if(text_separate() == -1) { 
		perror("text_separate failed\n");
		return EXIT_FAILURE;
	}
	if(text_binary_separate() == -1) { 
		perror("text_binary_separate failed\n");
		return EXIT_FAILURE;
	}
	/*
	if(pdf_separate() == -1) { 
		perror("pdf_separate failed\n");
		return EXIT_FAILURE;
	}
	if(docx_separate() == -1) { 
		perror("pdf_separate failed\n");
		return EXIT_FAILURE;
	}
	if(NETCDF_separate() == -1) { 
		perror("NetCDF_separate failed\n");
		return EXIT_FAILURE;
	}
	if(hdf5_separate() == -1) { 
		perror("hdf5_separate failed\n");
		return EXIT_FAILURE;
	}
	*/
	return EXIT_SUCCESS;
}
