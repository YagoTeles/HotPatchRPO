# 🔥 HotPatchRPO

**HotPatchRPO** é uma ferramenta gráfica para Windows que permite aplicar patches `.ptm` em arquivos `.rpo` do Protheus **sem precisar reiniciar o sistema ou desconectar usuários**.

---

## ⚙️ Funcionalidades

- Interface visual simples para selecionar:
  - Diretório APO original (com `custom.rpo` e `tttm120.rpo`)
  - Arquivo `.ptm`
  - Aplicador `.exe`
- Validação dos campos obrigatórios
- Cópia da pasta APO para novo diretório com timestamp
- Aplicação automática do `.ptm` via execução do `.exe` informado
- Ideal para uso em ambientes produtivos com usuários ativos

---

## 💡 Como funciona

1. O diretório APO atual é clonado para uma nova pasta com timestamp no nome (ex: `apo_240614153001`).
2. Apenas os arquivos `custom.rpo` e `tttm120.rpo` são copiados.
3. O arquivo `appserver.exe` é executado para aplicação do patch:


## 🧾 Licença

Distribuído sob a licença [MIT](LICENSE).

---

## ✍️ Autor

Yago Silva Teles
