function f=F(n)


if(n==0)
    f=0;
elseif(n==1)
    f=1;
else
    f=F(n-1)+F(n-2);
end
end

        
