To run the code:

```
$ gcc -O3 -mavx matrix_addition_simd.c
```

To run `dot_product`

```
gcc -O3 -mavx2 dot_product_simd.c
```

notes:

the matrix addition function that uses simd instructions is 83 percent faster.
