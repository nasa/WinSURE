program test(input,output);

var ans:double;
    p:array[0..10] of double;
    pc:array[0..10] of double;
    i,ierr:integer;

(* [external] *) function cgamma(x:double; var ans:double):integer; external;
(* [external] *) function lngamma(x:double; var ans:double):integer; external;

begin

   p[0] := 0.3;
   p[1] := 0.4;
   p[2] := 0.7;
   p[3] := 1.0;
   p[4] := 1.3;
   p[5] := 2.1;
   p[6] := 2.2;
   p[7] := 4.5;
   p[8] := 12.1;
   p[9] := 50.;
   p[10] := 0.71e15;

   pc[0] := 0.9;
   pc[1] := 1.5;
   pc[2] := 1.92;
   pc[3] := 2.0;
   pc[4] := 2.2;
   pc[5] := 2.95;
   pc[6] := 3.5;
   pc[7] := 11.87;
   pc[8] := 12.6;
   pc[9] := 167.51;
   pc[10] := 169.3;

   writeln('  p[i]          lngamma(p[i],&ans)       ierr');
   writeln('--------     ------------------------    ----');
   for i:=0 to 10 do begin
      ierr := lngamma(p[i],ans);
      writeln(p[i]:8:3,' ':4,ans:25:14,ierr:5);
      end;
   writeln(' pc[i]           cgamma(p[i],&ans)       ierr');
   writeln('--------     ------------------------    ----');
   for i:=0 to 10 do begin
      ierr := cgamma(pc[i],ans);
      writeln(p[i]:8:3,' ':4,ans:25:14,ierr:5);
      end;
end.
