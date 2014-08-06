#include <Magnum/Buffer.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Renderer.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData3D.h>

#include <Magnum/Context.h>
#include <Magnum/Version.h>

using namespace Magnum;

struct Client: public Platform::Application
{
	explicit Client(Arguments const &arguments)
	: Platform::Application(arguments, Configuration().setTitle("Recipe Spelunker Client"))
	{
		Debug()
			<< "This application is running on"
			<< Context::current()->version()
			<< "using"
			<< Context::current()->rendererString();

		Renderer::enable(Renderer::Feature::DepthTest);
		Renderer::enable(Renderer::Feature::FaceCulling);

		Trade::MeshData3D cube = Primitives::Cube::solid();

		vertexBuffer.setData(MeshTools::interleave(cube.positions(0), cube.normals(0)), BufferUsage::StaticDraw);

		Containers::Array<char> indexData;
		Mesh::IndexType indexType;
		UnsignedInt indexStart, indexEnd;
		std::tie(indexData, indexType, indexStart, indexEnd) = MeshTools::compressIndices(cube.indices());
		indexBuffer.setData(indexData, BufferUsage::StaticDraw);

		mesh
			.setPrimitive(cube.primitive())
			.setCount(cube.indices().size())
			.addVertexBuffer(vertexBuffer, 0, Shaders::Phong::Position{}, Shaders::Phong::Normal{})
			.setIndexBuffer(indexBuffer, 0, indexType, indexStart, indexEnd);

		transformation =
			Matrix4::rotationX(Deg(30.0f))*
			Matrix4::rotationY(Deg(40.0f));
		color = Color3::fromHSV(Deg(35.0f), 1.0f, 1.0f);

		projection =
			Matrix4::perspectiveProjection(Deg(35.0f), Vector2(defaultFramebuffer.viewport().size()).aspectRatio(), 0.01f, 100.0f)*
			Matrix4::translation(Vector3::zAxis(-10.0f));
	}

private:
	virtual void drawEvent() override
	{
		defaultFramebuffer.clear(FramebufferClear::Color|FramebufferClear::Depth);

		color = Color3::fromHSV(color.hue() + Deg(1.0), 1.0f, 1.0f);

		shader
			.setLightPosition({7.0f, 5.0f, 2.5f})
			.setLightColor(Color3(1.0f))
			.setDiffuseColor(color)
			.setAmbientColor(Color3::fromHSV(color.hue(), 1.0f, 0.3f))
			.setTransformationMatrix(transformation)
			.setNormalMatrix(transformation.rotationScaling())
			.setProjectionMatrix(projection);

		mesh.draw(shader);

		swapBuffers();

		redraw();
	}
	virtual void mousePressEvent(MouseEvent &e) override
	{
		if(e.button() != MouseEvent::Button::Left) return;

		previousMousePosition = e.position();
		e.setAccepted();
	}
	virtual void mouseReleaseEvent(MouseEvent &e) override
	{
		//color = Color3::fromHSV(color.hue() + Deg(50.0), 1.0f, 1.0f);

		e.setAccepted();
		redraw();
	}
	virtual void mouseMoveEvent(MouseMoveEvent &e) override
	{
		if(!(e.buttons() & MouseMoveEvent::Button::Left)) return;

		Vector2 delta = 3.0f*Vector2(e.position() - previousMousePosition)/Vector2(defaultFramebuffer.viewport().size());
		transformation =
			Matrix4::rotationX(Rad(delta.y()))*
			transformation*
			Matrix4::rotationY(Rad(delta.x()));

		previousMousePosition = e.position();
		e.setAccepted();
		redraw();
	}

	Buffer indexBuffer, vertexBuffer;
	Mesh mesh;
	Shaders::Phong shader;

	Matrix4 transformation, projection;
	Vector2i previousMousePosition;
	Color3 color;
};

MAGNUM_APPLICATION_MAIN(Client)
