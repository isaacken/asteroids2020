# Instruções para build do projeto

Siga o passo-a-passo abaixo para incluir a biblioteca **graphics.h** no Code::Blocks.

- Faça o download da WinBGIm no link http://winbgim.codecutter.org/
- Extraia e copie os arquivos **graphics.h** e **winbgim.h** para a pasta include do compilador (normalmente C:/Program Files/CodeBlocks/MinGW/include)
- Extraia e copie o arquivo **libbgi.a** para a pasta lib do compilador (normalmente C:/Program Files/CodeBlocks/MinGW/lib)
- Abra o Code::Blocks e acesse o menu *Settings >> Compiler >> Linker settings*
- Clique no botão *Add*, selecione *...* para buscar um arquivo e selecione o arquivo **libbgi.a** copiado anteriormente para a pasta lib
- Ainda nas configurações da aba *Linker settings*, cole o comando abaixo no campo "Other link options" e salve as alterações
`lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32`
- Por fim, se ao tentar compilar houver algum erro, abra o arquivos **graphics.h** copiado para a pasta include, e substitua a linha 302 pela linha abaixo
```C
    int left=0, int top=0, int right=INT_MAX, int bottom=INT_MAX,
```
- Compile o arquivo src.cpp
