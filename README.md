# AfriLang

## More about AfriLang
  I was intested in how interpreters worked and wanted to study them a bit more. I found the book crafting interpreters by Robert Nystrom. After reading to the book I thought about making an Afrikaans programming language. AfriLang works more like javascript where all objects do not require any property definitions to be able to access the properties. 

## Usage
```cmd
make run
```

It will compile a file called target and then run it with the input file of main.afrl that is in the main directory.

## Example
```
fun fib(n)
{
  as(n <= 2) terug n;
  terug fib(n - 2) + fib(n - 1);
}

lat begin = klok();
vertoon fib(35);
vertoon klok() - begin;
```

This is an implementation of the known fib function which finds the 35th fibonacci number.

## TODO's
Implement arrays, ++ operator and the ability to import code from other folders.
