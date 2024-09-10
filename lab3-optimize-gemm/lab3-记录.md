#### 对实验指导书末尾问题的回答

1. 多个c代码中有相同的MY_MMult函数，怎么判断可执行文件调用的是哪个版本的MY_MMult函数？是makefile中的哪行代码决定的？ 

从makefile文件的line 1, 2得到
```
    OLD  := MMult0
    NEW  := MMult1
```  
OLD和NEW后面带的文件名即为可执行文件调用的函数，做对应更改即可  
我已将其改为
```
    OLD  := openblas_MMult
    NEW  := My_MMult
```
<br>

2. 性能数据_data/output_MMult0.m是怎么生成的？c代码中只是将数据输出到终端并没有写入文件。  

makefile文件line 40 - 42:
```
	@echo "date = '`date`';" > $(DATA_DIR)/output_$(NEW).m
	@echo "version = '$(NEW)';" >> $(DATA_DIR)/output_$(NEW).m
	$(BUILD_DIR)/test_MMult.x >> $(DATA_DIR)/output_$(NEW).m
```
执行了将结果写入_data目录下对应的.m文件的操作  
<br><br>

#### 关于实验中遇到的问题  
1. 直接调用cblas_dgemm函数，编译器报错显示无法找到头文件  

gpt后得知，在makefile文件中加入下列代码
```
CFLAGS += -I/path/to/openblas/include
LDFLAGS += -L/path/to/openblas/lib -lopenblas
```
确保编译器能找到cblas的头文件和库文件，再次执行make run命令，能正常执行  

2. 运行python3 plotFlops.py, 没有图片且命令行报错  

ubuntu不支持图形化gui，选择将图片保存到本地查看，不用plt.show()
<br>  

3. 个人的疑问:实现的“高性能”运算完全不及openblas实现，在之后的课程中如何继续优化？

4. 所有可执行文件及output(_build, _data等)均已删除