# 高性能並列処理の作業用
### ログをここに書いていく

- コンパイル方法
```
mpicc <source file>
```

- 実行方法
```
mpirun -np <node_num> <exe file>
```

- ファイル構成
    - hello_world.c: openMPIのハローワールド
    - matrix_product.c: 行列計算(逐次処理)
    - matrix_product_parallel.c: 行列計算(並列処理)
    - それ以外: 授業配布用サンプルコード
