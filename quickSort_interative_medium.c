#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 交换两个元素的值
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition_median(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;

    // 确保arr[low] <= arr[mid] <= arr[high]
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

// 非递归快速排序（使用栈模拟递归）
void quickSort_iterative(int arr[], int low, int high) {
    // 创建栈来存储边界
    int stack[high - low + 1];
    int top = -1;

    // 初始边界入栈
    stack[++top] = low;
    stack[++top] = high;

    while (top >= 0) {
        // 出栈获取边界
        high = stack[top--];
        low = stack[top--];

        // 分区操作（可以用原来的partition函数）
        int pi = partition_median(arr, low, high);

        // 如果左边有元素，压入左边界
        if (pi - 1 > low) {
            stack[++top] = low;
            stack[++top] = pi - 1;
        }

        // 如果右边有元素，压入右边界
        if (pi + 1 < high) {
            stack[++top] = pi + 1;
            stack[++top] = high;
        }
    }
}
// 生成测试数据
void generate_test_data(int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("错误：无法创建文件 %s\n", filename);
        return;
    }

    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", rand() % 10000); 
    }

    fclose(file);
    printf("已生成 %d 条测试数据到 %s\n", size, filename);
}

// 从文件读取数据
int read_data_from_file(const char* filename, int arr[], int max_size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("错误：无法打开文件 %s\n", filename);
        return 0;
    }

    int count = 0;
    while (count < max_size && fscanf(file, "%d", &arr[count]) == 1) {
        count++;
    }

    fclose(file);
    return count;
}

// 检查数组是否已排序
int is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0; // 未排序
        }
    }
    return 1; // 已排序
}

// 打印数组前20个元素（用于预览）
void print_array_preview(int arr[], int n) {
    printf("前20个元素: ");
    int limit = n < 20 ? n : 20;
    for (int i = 0; i < limit; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    printf("=== 快速排序测试程序 ===\n");

    // 配置测试参数
    const char* filename = "test_data.txt";
    int data_size = 100000; 
    int* data = (int*)malloc(data_size * sizeof(int));

    if (data == NULL) {
        printf("错误：内存分配失败！\n");
        return 1;
    }

    // 1. 生成测试数据
    printf("\n1. 生成测试数据...\n");
    generate_test_data(data_size, filename);

    // 2. 读取数据
    printf("2. 读取数据...\n");
    int actual_size = read_data_from_file(filename, data, data_size);
    printf("   成功读取 %d 条数据\n", actual_size);
    print_array_preview(data, actual_size);

    // 3. 执行排序并计时
    printf("3. 开始快速排序...\n");
    clock_t start_time = clock();
    quickSort_iterative(data, 0, actual_size - 1);
    clock_t end_time = clock();

    // 4. 计算用时
    double time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // 5. 验证结果
    printf("4. 验证排序结果...\n");
    if (is_sorted(data, actual_size)) {
        printf("排序正确！\n");
    }
    else {
        printf(" 排序错误！\n");
    }

    printf("5. 性能统计：\n");
    printf("   - 数据量: %d 条\n", actual_size);
    printf("   - 排序用时: %.3f 秒\n", time_used);

    print_array_preview(data, actual_size);

    free(data);
    printf("\n=== 程序执行完毕 ===\n");
    return 0;
}