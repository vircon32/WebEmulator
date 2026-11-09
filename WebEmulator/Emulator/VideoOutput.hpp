// *****************************************************************************
    // start include guard
    #ifndef VIDEOOUTPUT_HPP
    #define VIDEOOUTPUT_HPP
    
    // include common Vircon headers
    #include "../VirconDefinitions/DataStructures.hpp"
    #include "../VirconDefinitions/Enumerations.hpp"
    
    // include console logic headers
    #include "ConsoleLogic/ExternalInterfaces.hpp"
    
    // include SDL2 headers
    #define SDL_MAIN_HANDLED
    #include "SDL.h"            // [ SDL2 ] Main header
    
    // include OpenGL headers; on desktop we would need a
    // loader such as glad, but since Emscripten provides
    // the symbols directly it is not needed here
    #include <GLES2/gl2.h>      // [ OpenGL ] GLES2 headers, compatible with WebGL1
// *****************************************************************************


// we will render our quads in groups using a fixed size queue;
// this parameter sets the queue size and acts as group size limit;
// on WebGL/JS the cost per call is quite higher so compensate by
// allowing longer drawing queues
#define QUAD_QUEUE_SIZE 512


// =============================================================================
//      2D-SPECIALIZED OPENGL CONTEXT
// =============================================================================


class VideoOutput
{
    private:
        
        // video context objects
        SDL_Window* Window;
        SDL_GLContext OpenGLContext;
        
        // graphical settings
        unsigned WindowWidth;
        unsigned WindowHeight;
        unsigned WindowedZoomFactor;
        
        // arrays to hold buffer info
        GLfloat QuadVerticesInfo[ 16 * QUAD_QUEUE_SIZE ];
        GLushort VertexIndices[ 6 * QUAD_QUEUE_SIZE ];
        
        // current color modifiers
        V32::GPUColor MultiplyColor;
        V32::IOPortValues BlendingMode;
        
        // OpenGL IDs of loaded textures
        GLuint BiosTextureID;
        GLuint CartridgeTextureIDs[ V32::Constants::GPUMaximumCartridgeTextures ];
        int32_t SelectedTexture;
        
        // white texture used to draw solid colors
        GLuint WhiteTextureID;
        
        // framebuffer object data
        GLuint FramebufferID;
        GLuint FBColorTextureID;
        unsigned FramebufferWidth;
        unsigned FramebufferHeight;
        
        // additional GL objects
        GLuint VBOVertexInfo;
        GLuint VBOIndices;
        GLuint ShaderProgramID;
        
        // rendering control for quad groups
        int QueuedQuads;
        
        // positions of shader parameters
        GLuint VertexInfoLocation;
        GLuint TextureUnitLocation;
        GLuint MultiplyColorLocation;
        
    public:
        
        // instance handling
        VideoOutput();
       ~VideoOutput();
        
        // init functions
        void CreateOpenGLWindow();
        void CreateFramebuffer();
        bool CompileShaderProgram();
        void CreateWhiteTexture();
        void InitRendering();
        
        // release functions
        void Destroy();
        
        // external context access
        SDL_Window* GetWindow();
        SDL_GLContext GetOpenGLContext();
        GLuint GetFramebufferID();
        
        // view configuration
        void SetWindowZoom( int ZoomFactor );
        int GetWindowZoom();
        float GetRelativeWindowWidth();
        
        // framebuffer render functions
        void RenderToScreen();
        void RenderToFramebuffer();
        void DrawFramebufferOnScreen();
        void BeginFrame();
        
        // color control functions
        void SetMultiplyColor( V32::GPUColor NewMultiplyColor );
        V32::GPUColor GetMultiplyColor();
        void SetBlendingMode( V32::IOPortValues BlendingMode );
        V32::IOPortValues GetBlendingMode();
        
        // render functions
        void ClearScreen( V32::GPUColor ClearColor );
        void AddQuadToQueue( const V32::GPUQuad& Quad );
        void RenderQuadQueue();
        
        // texture handling
        void LoadTexture( int GPUTextureID, void* Pixels );
        void UnloadTexture( int GPUTextureID );
        void SelectTexture( int GPUTextureID );
        int32_t GetSelectedTexture();
};


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
