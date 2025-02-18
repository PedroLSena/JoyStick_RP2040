Aqui está um exemplo de **README.md** para o seu projeto. Ele descreve o propósito do projeto, como configurar o ambiente, como compilar e executar o código, e outras informações relevantes.

---

# Projeto de Controle de Joystick com Display e LEDs

Este projeto utiliza a placa **BitDogLab** com o microcontrolador **RP2040** para criar um sistema interativo que controla um quadrado na tela de um display **SSD1306** usando um joystick. Além disso, os valores do joystick são usados para controlar a intensidade de LEDs RGB via PWM. O projeto também inclui funcionalidades adicionais, como a alternância de estilos de borda no display e o controle de LEDs através de botões.

---

## Funcionalidades

1. **Controle de Quadrado no Display**:
   - O joystick controla a posição de um quadrado na tela do display SSD1306.
   - O quadrado não ultrapassa os limites da tela.

2. **Controle de LEDs RGB**:
   - O eixo X do joystick controla a intensidade do LED Vermelho.
   - O eixo Y do joystick controla a intensidade do LED Azul.
   - O LED Verde é alternado ao pressionar o botão do joystick.

3. **Estilos de Borda no Display**:
   - Ao pressionar o botão do joystick, o estilo da borda do display alterna entre:
     - Sem borda.
     - Borda simples.
     - Borda dupla.

4. **Controle de LEDs PWM**:
   - O botão A ativa/desativa o controle de intensidade dos LEDs Vermelho e Azul.

---

## Componentes Utilizados

- **Placa BitDogLab** com RP2040.
- **Display OLED SSD1306** (128x64 pixels) conectado via I2C.
- **Joystick analógico** conectado aos pinos ADC.
- **LEDs RGB** conectados aos pinos PWM.
- **Botões** para controle adicional.

---

## Configuração do Ambiente

### Dependências

Para compilar e executar este projeto, você precisará das seguintes ferramentas:

1. **SDK do Raspberry Pi Pico**:
   - Instale o [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk).

2. **Compilador GCC para ARM**:
   - Instale o compilador ARM (`arm-none-eabi-gcc`).

3. **Ferramentas de Build**:
   - Certifique-se de ter o `cmake` e o `make` instalados.

4. **Biblioteca SSD1306**:
   - A biblioteca para controlar o display SSD1306 está incluída no projeto.

### Conexões

| Componente   | Pino no RP2040 |
|--------------|----------------|
| Display SDA  | GPIO 14        |
| Display SCL  | GPIO 15        |
| Joystick X   | GPIO 26 (ADC0) |
| Joystick Y   | GPIO 27 (ADC1) |
| LED Vermelho | GPIO 13 (PWM)  |
| LED Verde    | GPIO 11        |
| LED Azul     | GPIO 12 (PWM)  |
| Botão Joystick | GPIO 22      |
| Botão A      | GPIO 5         |

---

## Compilação e Execução

Siga os passos abaixo para compilar e executar o projeto:

1. **Clone o repositório**:
   ```bash
   git clone https://github.com/seu-usuario/seu-projeto.git
   cd seu-projeto
   ```

2. **Configure o projeto**:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. **Compile o projeto**:
   ```bash
   make
   ```

4. **Carregue o firmware na placa**:
   - Conecte a placa BitDogLab ao computador via USB.
   - Copie o arquivo `.uf2` gerado para a placa:
     ```bash
     cp projeto.uf2 /media/seu-usuario/RPI-RP2/
     ```

5. **Execute o projeto**:
   - A placa reiniciará e executará o código automaticamente.

---

## Estrutura do Código

- **main.c**: Contém a lógica principal do projeto, incluindo o controle do joystick, LEDs e display.
- **ssd1306.h**: Biblioteca para controlar o display SSD1306.
- **CMakeLists.txt**: Configuração do projeto para compilação com CMake.

---

## Explicação do Código

### Funções Principais

1. **`joystick_control`**:
   - Lê os valores do joystick e atualiza a posição do quadrado no display.
   - Atualiza a intensidade dos LEDs com base nos valores do joystick.

2. **`update_leds`**:
   - Controla a intensidade dos LEDs Vermelho e Azul usando PWM.

3. **`draw_border`**:
   - Desenha a borda no display de acordo com o estilo atual.

4. **`button_irq`**:
   - Trata as interrupções dos botões, alternando o LED Verde e o estilo da borda.

---

## Vídeo de Demonstração

[Link para o vídeo de demonstração](#) *(adicione um link para um vídeo mostrando o projeto em funcionamento)*.

---

## Licença

Este projeto está licenciado sob a licença MIT. Consulte o arquivo [LICENSE](LICENSE) para mais detalhes.

---

## Contribuições

Contribuições são bem-vindas! Siga os passos abaixo:

1. Faça um fork do projeto.
2. Crie uma branch para sua feature (`git checkout -b feature/nova-feature`).
3. Commit suas mudanças (`git commit -m 'Adiciona nova feature'`).
4. Faça push para a branch (`git push origin feature/nova-feature`).
5. Abra um Pull Request.

---

## Contato

Se você tiver alguma dúvida ou sugestão, sinta-se à vontade para entrar em contato:


---

Esse README fornece uma visão geral do projeto e instruções claras para configurar, compilar e executar o código. Adapte as informações conforme necessário para o seu caso específico. 😊