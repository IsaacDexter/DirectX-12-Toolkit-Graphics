//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "map"


// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game();

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize(int& width, int& height) const noexcept;

private:

    void Update(DX::StepTimer const& timer);

    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    void LoadTextures();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    /// <summary><para>Manages video memory  allocations</para>
    /// <para>Call commit after presenting buffers to track and free memory</para>
    /// <para>Ensure initialization when creating resources</para></summary>
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    /// <summary>Stores and allocates objects needed by shaders</summary>
    std::unique_ptr<DirectX::DescriptorPile> m_srvPile;


    RECT m_fullscreenRect;

    /// <summary><para>desc: The Texture's descriptor</para>
    /// <para>slot: The Texture's slot for accessing it's resource in EffectTextureFactory</para></summary>
    struct TexHand
    {
        size_t desc;
        size_t slot;
        /// <param name="desc">The Texture's descriptor</param>
        /// <param name="slot">The Texture's slot for accessing it's resource in EffectTextureFactory</param>
        TexHand(size_t desc, size_t slot)
        {
            this->desc = desc;
            this->slot = slot;
        }
    };

    std::unique_ptr<DirectX::EffectTextureFactory> m_textureResources;
    /// <summary>Maps the name of a texture to its handles (slot and descriptor) for use with an EffectTextureFactory.</summary>
    std::unique_ptr<std::map<const wchar_t*, TexHand>> m_texHands;
    std::vector<const wchar_t*> m_textureLoadList;


    enum Descriptors
    {
        Gui,
        RenderTexture,
        Reserve,
        Count = 128
    };

    enum RTDescriptors
    {
        OffscreenRT,
        RTCount
    };

    /// <summary>Helper that handles additional D3D resources required for drawing</summary>
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

    DirectX::SimpleMath::Vector2 m_origin;

    std::unique_ptr<DirectX::CommonStates> m_states;

    // select the vertex input layout
    using VertexType = DirectX::VertexPositionNormalTexture;
    //provides root signature and PSO
    std::unique_ptr<DirectX::NormalMapEffect> m_effect;
    // provides vertex buffer and primitive topology
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;

    //Do the same for the wireframe effect and batch
    using WireframeVertexType = DirectX::VertexPositionColor;
    std::unique_ptr<DirectX::BasicEffect> m_wireframeEffect;
    std::unique_ptr<DirectX::PrimitiveBatch<WireframeVertexType>> m_wireframeBatch;

    // world view and projection matrices
    DirectX::SimpleMath::Matrix m_world;
    DirectX::SimpleMath::Matrix m_view;
    DirectX::SimpleMath::Matrix m_proj;
    
    // camera values
    float m_pitch;
    float m_yaw;
    DirectX::SimpleMath::Vector3 m_cameraPos;
    float m_rotationGain = 0.1;
    float m_movementGain = 1.0f;
    DirectX::SimpleMath::Vector3 m_startPos = { 0.0f, 2.f, 2.f };
    

    // geometric primitive for rendering simple sphere
    std::unique_ptr<DirectX::GeometricPrimitive> m_shape;

    // rendering to texture
    std::unique_ptr<DirectX::DescriptorHeap> m_renderDescriptors;
    std::unique_ptr<DX::RenderTexture> m_renderTexture;

    // keyboard and mouse input
    std::unique_ptr<DirectX::Keyboard> m_keyboard;
    std::unique_ptr<DirectX::Mouse> m_mouse;
};
