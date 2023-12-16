# Description Turkish

Circular_Buffer ve Circular_Queue içermektedir.
Buffer uzunluğu 512 ve Queue uzunluğu 16 tane 20 karakter uzunluktan oluşmaktadır.

 Interrupt ile uart üzerinden gelen bütün verileri uzunluğu 512 olan buffer içerisine almaktayız.(Head artıyor). 
Eğer özel karakter olarak belirlenen \n gelirse gelen tüm veriler sıradaki queue içerisine alınmaktadır. (Tail artıyor) 
Eğer Head 512'ye ulaşırsa 0 yapılıp circular yapı elde ediliyor aynı şekilde queue'de 20 ye ulaşırsa 0 yapılıp circular queue yapılıyor.

Circular buffer uart'dan alınan verilerin alınmasını kolaylaştırıyor ve veri kayıplarının önüne geçmeyi hedefliyor.

 Circular queue stm32 f401RE üzerinde bulunan dahili led'i uart üzerinden kontrol etmek için projeye uyarladım.
 Led'i kapatıp açma , blink efekti ve kapanıp açılma efektininin sürelerini uart'dan gelen veriler ile kontrol etmeyi amaçlıyor. 
Circular buffer kullanmadan ve daha sonra circular buffer kullanarak led'i kontrol etmeyi deneyerek circular_buffer'in önemini daha iyi anladım.

Özetle geliştirme amacım circular bufffer'in calısma mantıgını anlamaktı ve çok faydalı oldu.

# Description English 

The project contains Circular_Buffer and Circular_Queue.
The buffer length is 512, and the queue length is 16, each consisting of 20 characters.

With the interrupt, all data coming from UART is placed in the buffer with a length of 512 (Head is incremented). If the special character '\n' is received, all the data is placed in the next queue (Tail is incremented). If Head reaches 512, it is set to 0, creating a circular structure. Similarly, if the queue reaches 20, it is set to 0, creating a circular queue.

Circular buffer facilitates the reception of data from UART and aims to prevent data loss.

I adapted Circular Queue to control the internal LED on the STM32 F401RE through UART in the project. It aims to control turning off and on the LED, blinking effects, and the durations of turning off and on with data received from UART. By experimenting with controlling the LED without using Circular Buffer and then using Circular Buffer, I better understood the importance of Circular Buffer.

In summary, my development goal was to understand the working logic of the circular buffer, and it proved to be very useful.
