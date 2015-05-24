CREATE OR REPLACE FUNCTION MID.IpToLong(ip in varchar2)
return number
is
  p1 number;
  p2 number;
  p3 number;
begin
  p1 := instr(ip, '.', 1, 1);
  p2 := instr(ip, '.', 1, 2);
  p3 := instr(ip, '.', 1, 3);
  return to_number(substr(ip, 1, p1-1))*256*256*256 + to_number(substr(ip, p1+1, p2-p1-1))*256*256 + to_number(substr(ip, p2+1, p3-p2-1))*256 + to_number(substr(ip, p3+1));
end;
例子:select mid.iptolong('192.168.0.1') ip  from dual
    IP
3232235521
--把long型的ip转换成ip格式
CREATE OR REPLACE FUNCTION MID.LongToIp(ip in number)
return varchar2
is
begin
return floor(ip/65536/256) || '.' || mod(floor(ip/65536), 256) || '.' || mod(floor(ip/256), 256) || '.' || mod(ip, 256);
end;
例子:select mid.longtoip(3232235521) ip from dual
    IP
192.168.0.1
