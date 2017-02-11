module top;

    typedef struct { 
        bit   [11:0] x;
        logic [0:11] y;
    } MyType; 

    typedef logic[3:0]  My1DLV[5:0];
    typedef logic[3:0]  My2DLV[7:0][5:0];
    typedef logic[0:3]  My3DLV[11:0][7:0][5:0];

    typedef bit[3:0]  My1DBV[5:0];
    typedef bit[3:0]  My2DBV[7:0][5:0];
    typedef bit[0:3]  My3DBV[11:0][7:0][5:0];

    typedef bit  My1DBS[5:0];
    typedef bit  My2DBS[7:0][5:0];
    typedef bit  My3DBS[11:0][7:0][5:0];

    typedef logic  My1DLS[5:0];
    typedef logic  My2DLS[7:0][5:0];
    typedef logic  My3DLS[11:0][7:0][5:0];

    import "DPI-C" function void f0(input MyType i [], output MyType o []); 
    import "DPI-C" function void f1(input MyType i [], output MyType o []); 
    import "DPI-C" function void f2(input MyType i [], output MyType o []); 

    // vector

    import "DPI-C" function void f3a(input bit[] i [], output bit[] o []); 
    import "DPI-C" function void f3b(input bit[] i [], output bit[] o []); 

    import "DPI-C" function void f4a(input bit[] i [][], output bit[] o [][]); 
    import "DPI-C" function void f4b(input bit[] i [][], output bit[] o [][]); 

    import "DPI-C" function void f5a(input bit[3:0] i [][][], output bit[3:0] o [][][]); 
    import "DPI-C" function void f5b(input bit[3:0] i [][][], output bit[3:0] o [][][]); 

    import "DPI-C" function void f6a(input logic[] i [], output logic[] o []); 
    import "DPI-C" function void f6b(input logic[] i [], output logic[] o []); 

    import "DPI-C" function void f7a(input logic[] i [][], output logic[] o [][]); 
    import "DPI-C" function void f7b(input logic[] i [][], output logic[] o [][]); 

    import "DPI-C" function void f8a(input logic[3:0] i [][][], output logic[3:0] o [][][]); 
    import "DPI-C" function void f8b(input logic[3:0] i [][][], output logic[3:0] o [][][]); 

    // scalar

    import "DPI-C" function void f9a(input bit[] i [], output bit[] o []); 
    import "DPI-C" function void f9b(input bit[] i [][], output bit[] o [][]); 
    import "DPI-C" function void f9c(input bit[] i [][][], output bit[] o [][][]); 

    import "DPI-C" function void f10a(input logic[] i [], output logic[] o []); 
    import "DPI-C" function void f10b(input logic[] i [][], output logic[] o [][]); 
    import "DPI-C" function void f10c(input logic[] i [][][], output logic[] o [][][]); 


    MyType source [11:20] = '{'{1, 9}, '{2, 8}, '{3, 7}, '{4, 6}, '{5, 5}, '{6, 4}, '{7, 3}, '{8, 2}, '{9, 1}, '{0, 10}} ; 
    MyType target [11:20]; 

    My1DLV lv1d, lv1d_o;
    My2DLV lv2d, lv2d_o;
    My3DLV lv3d, lv3d_o;

    My1DBV bv1d, bv1d_o;
    My2DBV bv2d, bv2d_o;
    My3DBV bv3d, bv3d_o;

    My1DBS bs1d, bs1d_o;
    My2DBS bs2d, bs2d_o;
    My3DBS bs3d, bs3d_o;
    My1DLS ls1d, ls1d_o;
    My2DLS ls2d, ls2d_o;
    My3DLS ls3d, ls3d_o;

    initial begin
        int i;

        f0(source, target); 
        $display("f0: %p", target);

        f1(source, target); 
        $display("f1: %p", target);

        f2(source, target); 
        $display("f2: %p", target);

        for (i = 0; i < 6; ++i) begin
             lv1d[i] = i+3;
             lv2d[0][i] = i+3;
             lv3d[0][0][i] = i+3;

             bv1d[i] = i+3;
             bv2d[0][i] = i+3;
             bv3d[0][0][i] = i+3;

             if (i == 0) begin
                 bs1d[i] = 0;
                 bs2d[0][i] = 0;
                 bs3d[0][0][i] = 0;

                 ls1d[i] = 1;
                 ls2d[0][i] = 1;
                 ls3d[0][0][i] = 1;
             end else begin
                 bs1d[i] = !bs1d[i-1];
                 bs2d[0][i] = !bs2d[0][i-1];
                 bs3d[0][0][i] = !bs3d[0][0][i-1];

                 ls1d[i] = !ls1d[i-1];
                 ls2d[0][i] = !ls2d[0][i-1];
                 ls3d[0][0][i] = !ls3d[0][0][i-1];
             end
        end
  
        f3a(bv1d, bv1d_o);   
	$display("f3a: %p, expecting %p", bv1d_o, bv1d);

        f3b(bv1d, bv1d_o);   
	$display("f3b: %p, expecting %p", bv1d_o, bv1d);

        f4a(bv2d, bv2d_o);   
	$display("f4a: %p, expecting %p", bv2d_o[0], bv2d[0]);

        f4b(bv2d, bv2d_o);   
	$display("f4b: %p, expecting %p", bv2d_o[0], bv2d[0]);

        f5a(bv3d, bv3d_o);   
	$display("f5a: %p, expecting %p", bv3d_o[0][0], bv3d[0][0]);

        f5b(bv3d, bv3d_o);   
	$display("f5b: %p, expecting %p", bv3d_o[0][0], bv3d[0][0]);

        f6a(lv1d, lv1d_o);   
	$display("f6a: %p, expecting %p", lv1d_o, lv1d);

        f6b(lv1d, lv1d_o);   
	$display("f6b: %p, expecting %p", lv1d_o, lv1d);

        f7a(lv2d, lv2d_o);   
	$display("f7a: %p, expecting %p", lv2d_o[0], lv2d[0]);

        f7b(lv2d, lv2d_o);   
	$display("f7b: %p, expecting %p", lv2d_o[0], lv2d[0]);

        f8a(lv3d, lv3d_o);   
	$display("f8a: %p, expecting %p", lv3d_o[0][0], lv3d[0][0]);

        f8b(lv3d, lv3d_o);   
	$display("f8b: %p, expecting %p", lv3d_o[0][0], lv3d[0][0]);

        f9a(bs1d, bs1d_o);   
	$display("f9a: %p, expecting %p", bs1d_o, bs1d);
        f9b(bs2d, bs2d_o);   
	$display("f9b: %p, expecting %p", bs2d_o[0], bs2d[0]);
        f9c(bs3d, bs3d_o);   
	$display("f9c: %p, expecting %p", bs3d_o[0][0], bs3d[0][0]);

        f10a(ls1d, ls1d_o);   
	$display("f10a: %p, expecting %p", ls1d_o, ls1d);
        f10b(ls2d, ls2d_o);   
	$display("f10b: %p, expecting %p", ls2d_o[0], ls2d[0]);
        f10c(ls3d, ls3d_o);   
	$display("f10c: %p, expecting %p", ls3d_o[0][0], ls3d[0][0]);

    end
endmodule
