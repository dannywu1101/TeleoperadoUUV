#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "std_msgs/Float64MultiArray.h"
#include <array>
#include <iostream>

// Clase para el nodo teleoperado que maneja los motores
class NodoTeleoperado {
public:
    NodoTeleoperado() {
        // Inicializar la matriz de localización llena de "1"
        for (auto& row : matriz_localizacion) {
            row.fill(1);
        }

        // Suscriptor para el joystick
        joy_sub = nh.subscribe("/joy", 10, &NodoTeleoperado::joyCallback, this);

        // Publicador para los comandos de los motores
        motor_pub = nh.advertise<std_msgs::Float64MultiArray>("motores_comandos", 10);
    }

    // Callback que se activa al recibir datos del joystick
    void joyCallback(const sensor_msgs::Joy::ConstPtr& joy) {
        // Asignar valores del joystick a los ejes de control
        float eje_x = joy->axes[0]; // Ejemplo: eje izquierdo horizontal
        float eje_y = joy->axes[1]; // Ejemplo: eje izquierdo vertical
        float eje_z = joy->axes[3]; // Ejemplo: eje derecho horizontal para giro
        float velocidad_z = joy->axes[4]; // Ejemplo: eje derecho vertical para subir/bajar

        // Generar comandos de motores usando la matriz de localización
        calcularComandosMotores(eje_x, eje_y, eje_z, velocidad_z);
    }

    // Método para calcular los comandos de los motores
    void calcularComandosMotores(float x, float y, float z, float vz) {
        std_msgs::Float64MultiArray comandos;
        comandos.data.resize(6);

        // Ejemplo de lógica de movimiento en los motores usando la matriz de localización
        // Aquí se podría ajustar cada comando en función de la matriz, pero por ahora solo se usa de ejemplo.
        comandos.data[0] = (matriz_localizacion[0][0] * x) + (matriz_localizacion[0][1] * y) - (matriz_localizacion[0][2] * z) + vz; // Motor 1
        comandos.data[1] = (matriz_localizacion[1][0] * x) - (matriz_localizacion[1][1] * y) + (matriz_localizacion[1][2] * z) - vz; // Motor 2
        comandos.data[2] = (-matriz_localizacion[2][0] * x) + (matriz_localizacion[2][1] * y) + (matriz_localizacion[2][2] * z) + vz; // Motor 3
        comandos.data[3] = (-matriz_localizacion[2][0] * x) - (matriz_localizacion[2][1] * y) - (matriz_localizacion[2][2] * z) - vz; // Motor 4
        comandos.data[4] = matriz_localizacion[1][1] * vz; // Motor 5 (control de altura)
        comandos.data[5] = -matriz_localizacion[0][0] * vz; // Motor 6 (control de altura opuesto)

        motor_pub.publish(comandos);
        ROS_INFO("Comandos de motores enviados.");
    }

    // Método para imprimir la matriz de localización (solo para verificación)
    void imprimirMatriz() {
        ROS_INFO("Matriz de localización:");
        for (const auto& row : matriz_localizacion) {
            for (const auto& val : row) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    ros::NodeHandle nh;
    ros::Subscriber joy_sub;
    ros::Publisher motor_pub;
    std::array<std::array<int, 3>, 3> matriz_localizacion; // Matriz de localización de 3x3 llena de "1"
};

int main(int argc, char **argv) {
    ros::init(argc, argv, "nodo_teleoperado");

    NodoTeleoperado nodo;
    nodo.imprimirMatriz(); // Imprime la matriz de localización para verificar

    ros::spin();
    return 0;
}
