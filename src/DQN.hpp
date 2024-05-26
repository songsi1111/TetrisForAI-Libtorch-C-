# include<torch/torch.h>
# include<iostream>

class DQN : public torch::nn::Module{
public:
    torch::nn::Linear conv1{nullptr}, conv2{nullptr}, conv3{nullptr};
public:
    //定义3层隐藏神经网络
    DQN():torch::nn::Module(){
        conv1=register_module("conv1",torch::nn::Linear(4,64));
        conv2=register_module("conv2",torch::nn::Linear(64,64));
        conv3=register_module("conv3",torch::nn::Linear(64,1));
 
        _create_weight();
    }
    //初始化网络中的权重和偏置
    void _create_weight(){
        for(auto&module:modules(/*include_self=*/false)){
            //遍历线性层模块，初始化权重和偏置
            if(auto linear=module->as<torch::nn::Linear>()){
                torch::nn::init::xavier_uniform_(linear->weight);
                torch::nn::init::constant_(linear->bias,0);
            }
        }
    }
    //前向传播，得到参数的映射
    torch::Tensor forward(torch::Tensor x){
        x = conv1->forward(x);
        x = torch::relu(x);
        x = conv2->forward(x);
        x = torch::relu(x);
        x = conv3->forward(x);
        
        return x;
    }
    // 实现保存和加载方法
    void save(const std::string& file_path) {
        torch::save(this->conv1, file_path + "_conv1.pt");
        torch::save(this->conv2, file_path + "_conv2.pt");
        torch::save(this->conv3, file_path + "_conv3.pt");
    }

    void load(const std::string& file_path) {
        torch::load(this->conv1, file_path + "_conv1.pt");
        torch::load(this->conv2, file_path + "_conv2.pt");
        torch::load(this->conv3, file_path + "_conv3.pt");
    }
    // 重载()运算符,使DQN对象可以像函数一样调用
    torch::Tensor operator()(torch::Tensor x) {
        return forward(x);
    }
//     void save(const std::string& file_path) {
//         torch::save(*this, file_path);
//     }

//     void load(const std::string& file_path) {
//         torch::load(*this, file_path);
//     }
};

