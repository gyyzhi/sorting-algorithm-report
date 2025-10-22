#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ��������Ԫ�ص�ֵ
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition_median(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;

    // ȷ��arr[low] <= arr[mid] <= arr[high]
    if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
    if (arr[low] > arr[high]) swap(&arr[low], &arr[high]);
    if (arr[mid] > arr[high]) swap(&arr[mid], &arr[high]);

    swap(&arr[mid], &arr[high]);
    int pivot = arr[high];
    int i = (low - 1); 

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++; 
            swap(&arr[i], &arr[j]); 
        }
    }
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
}

// �ǵݹ��������ʹ��ջģ��ݹ飩
void quickSort_iterative(int arr[], int low, int high) {
    // ����ջ���洢�߽�
    int stack[high - low + 1];
    int top = -1;

    // ��ʼ�߽���ջ
    stack[++top] = low;
    stack[++top] = high;

    while (top >= 0) {
        // ��ջ��ȡ�߽�
        high = stack[top--];
        low = stack[top--];

        // ����������������ԭ����partition������
        int pi = partition_median(arr, low, high);

        // ��������Ԫ�أ�ѹ����߽�
        if (pi - 1 > low) {
            stack[++top] = low;
            stack[++top] = pi - 1;
        }

        // ����ұ���Ԫ�أ�ѹ���ұ߽�
        if (pi + 1 < high) {
            stack[++top] = pi + 1;
            stack[++top] = high;
        }
    }
}
// ���ɲ�������
void generate_test_data(int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("�����޷������ļ� %s\n", filename);
        return;
    }

    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", rand() % 10000); 
    }

    fclose(file);
    printf("������ %d ���������ݵ� %s\n", size, filename);
}

// ���ļ���ȡ����
int read_data_from_file(const char* filename, int arr[], int max_size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("�����޷����ļ� %s\n", filename);
        return 0;
    }

    int count = 0;
    while (count < max_size && fscanf(file, "%d", &arr[count]) == 1) {
        count++;
    }

    fclose(file);
    return count;
}

// ��������Ƿ�������
int is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0; // δ����
        }
    }
    return 1; // ������
}

// ��ӡ����ǰ20��Ԫ�أ�����Ԥ����
void print_array_preview(int arr[], int n) {
    printf("ǰ20��Ԫ��: ");
    int limit = n < 20 ? n : 20;
    for (int i = 0; i < limit; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    printf("=== ����������Գ��� ===\n");

    // ���ò��Բ���
    const char* filename = "test_data.txt";
    int data_size = 100000; 
    int* data = (int*)malloc(data_size * sizeof(int));

    if (data == NULL) {
        printf("�����ڴ����ʧ�ܣ�\n");
        return 1;
    }

    // 1. ���ɲ�������
    printf("\n1. ���ɲ�������...\n");
    generate_test_data(data_size, filename);

    // 2. ��ȡ����
    printf("2. ��ȡ����...\n");
    int actual_size = read_data_from_file(filename, data, data_size);
    printf("   �ɹ���ȡ %d ������\n", actual_size);
    print_array_preview(data, actual_size);

    // 3. ִ�����򲢼�ʱ
    printf("3. ��ʼ��������...\n");
    clock_t start_time = clock();
    quickSort_iterative(data, 0, actual_size - 1);
    clock_t end_time = clock();

    // 4. ������ʱ
    double time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // 5. ��֤���
    printf("4. ��֤������...\n");
    if (is_sorted(data, actual_size)) {
        printf("������ȷ��\n");
    }
    else {
        printf(" �������\n");
    }

    printf("5. ����ͳ�ƣ�\n");
    printf("   - ������: %d ��\n", actual_size);
    printf("   - ������ʱ: %.3f ��\n", time_used);

    print_array_preview(data, actual_size);

    free(data);
    printf("\n=== ����ִ����� ===\n");
    return 0;
}