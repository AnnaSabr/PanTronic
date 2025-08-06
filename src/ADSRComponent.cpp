/**
 * @file ADSRComponent.cpp
 * @brief Implementation of the ADSRComponent class
 */

#include "ADSRComponent.hpp"

ADSRComponent::ADSRComponent()
{
    // Set default size and enable mouse interaction
    setSize(400, 150);
    setInterceptsMouseClicks(true, false);
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(10);

    // Draw background with transparency
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.fillRoundedRectangle(bounds, 5.0f);

    // Draw border
    g.setColour(primaryColor.withAlpha(0.5f));
    g.drawRoundedRectangle(bounds, 5.0f, 2.0f);

    // Draw grid lines (optional visual aid)
    g.setColour(secondaryColor.withAlpha(0.2f));
    for (int i = 1; i < 4; ++i)
    {
        float y = bounds.getY() + (bounds.getHeight() / 4.0f) * i;
        g.drawLine(bounds.getX(), y, bounds.getRight(), y, 1.0f);
    }

    // Create and draw ADSR curve
    auto path = createADSRPath();

    // Draw filled area under the curve
    auto filledPath = path;
    filledPath.lineTo(bounds.getRight(), bounds.getBottom());
    filledPath.lineTo(bounds.getX(), bounds.getBottom());
    filledPath.closeSubPath();

    g.setColour(primaryColor.withAlpha(0.2f));
    g.fillPath(filledPath);

    // Draw ADSR curve line
    g.setColour(primaryColor);
    g.strokePath(path, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved));

    // Get control point positions
    auto attackPoint = getAttackPoint();
    auto decayPoint = getDecayPoint();
    auto sustainPoint = getSustainPoint();
    auto releasePoint = getReleasePoint();

    /**
     * @brief Lambda function to draw control points
     * @param point Position of the control point
     * @param isActive Whether this point is currently being dragged
     */
    auto drawControlPoint = [&](juce::Point<float> point, bool isActive) {
        float radius = isActive ? 8.0f : 6.0f;
        g.setColour(isActive ? primaryColor : primaryColor.withAlpha(0.8f));
        g.fillEllipse(point.x - radius/2, point.y - radius/2, radius, radius);

        g.setColour(juce::Colours::white);
        g.drawEllipse(point.x - radius/2, point.y - radius/2, radius, radius, 2.0f);
    };

    // Draw all control points
    drawControlPoint(attackPoint, currentDragMode == DragMode::Attack);
    drawControlPoint(decayPoint, currentDragMode == DragMode::Decay);
    drawControlPoint(sustainPoint, currentDragMode == DragMode::Sustain);
    drawControlPoint(releasePoint, currentDragMode == DragMode::Release);
}

void ADSRComponent::resized()
{
    // Called when component size changes - currently no specific layout needed
}

void ADSRComponent::mouseDown(const juce::MouseEvent& event)
{
    // Store initial mouse position and determine which control point was hit
    lastMousePos = event.position;
    currentDragMode = getHitTest(event.position);

    // Repaint if we started dragging a control point
    if (currentDragMode != DragMode::None)
    {
        repaint();
    }
}

void ADSRComponent::mouseDrag(const juce::MouseEvent& event)
{
    // Exit if no active drag mode
    if (currentDragMode == DragMode::None)
        return;

    auto bounds = getLocalBounds().toFloat().reduced(10);
    auto delta = event.position - lastMousePos;

    // Sensitivity factor for mouse movement to parameter change conversion
    const float sensitivity = 0.003f;

    // Update the appropriate parameter based on drag mode
    switch (currentDragMode)
    {
        case DragMode::Attack:
            // Horizontal movement controls attack time
            attackValue = juce::jlimit(0.01f, 1.0f, attackValue + delta.x * sensitivity);
            break;

        case DragMode::Decay:
            // Horizontal movement controls decay time
            decayValue = juce::jlimit(0.01f, 1.0f, decayValue + delta.x * sensitivity);
            break;

        case DragMode::Sustain:
            // Vertical movement controls sustain level (inverted)
            sustainValue = juce::jlimit(0.0f, 1.0f, sustainValue - delta.y * sensitivity);
            break;

        case DragMode::Release:
            // Horizontal movement controls release time
            releaseValue = juce::jlimit(0.01f, 1.0f, releaseValue + delta.x * sensitivity);
            break;

        default:
            break;
    }

    // Update mouse position for next delta calculation
    lastMousePos = event.position;

    // Notify external listeners of parameter changes
    if (onParameterChanged)
    {
        onParameterChanged(attackValue, decayValue, sustainValue, releaseValue);
    }

    // Trigger visual update
    repaint();
}

void ADSRComponent::mouseUp(const juce::MouseEvent& event)
{
    // End drag mode and update visual state
    currentDragMode = DragMode::None;
    repaint();
}

void ADSRComponent::mouseMove(const juce::MouseEvent& event)
{
    // Check if mouse is over a control point and update cursor accordingly
    auto hitTest = getHitTest(event.position);

    if (hitTest != DragMode::None)
    {
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    }
    else
    {
        setMouseCursor(juce::MouseCursor::NormalCursor);
    }
}

void ADSRComponent::setAttack(float attack)
{
    attackValue = juce::jlimit(0.01f, 1.0f, attack);
    repaint();
}

void ADSRComponent::setDecay(float decay)
{
    decayValue = juce::jlimit(0.01f, 1.0f, decay);
    repaint();
}

void ADSRComponent::setSustain(float sustain)
{
    sustainValue = juce::jlimit(0.0f, 1.0f, sustain);
    repaint();
}

void ADSRComponent::setRelease(float release)
{
    releaseValue = juce::jlimit(0.01f, 1.0f, release);
    repaint();
}

void ADSRComponent::updateColors(juce::Colour primary, juce::Colour secondary)
{
    primaryColor = primary;
    secondaryColor = secondary;
    repaint();
}

juce::Path ADSRComponent::createADSRPath() const
{
    auto bounds = getLocalBounds().toFloat().reduced(10);
    juce::Path path;

    // Start point (0, bottom) - envelope starts at zero
    float startX = bounds.getX();
    float startY = bounds.getBottom();
    path.startNewSubPath(startX, startY);

    // Attack phase - rise to peak
    auto attackPoint = getAttackPoint();
    path.lineTo(attackPoint);

    // Decay phase - fall to sustain level
    auto decayPoint = getDecayPoint();
    path.lineTo(decayPoint);

    // Sustain phase - horizontal line at sustain level
    auto sustainPoint = getSustainPoint();
    path.lineTo(sustainPoint);

    // Release phase - fall back to zero
    auto releasePoint = getReleasePoint();
    path.lineTo(releasePoint);

    return path;
}

ADSRComponent::DragMode ADSRComponent::getHitTest(juce::Point<float> position) const
{
    const float hitRadius = 10.0f;

    // Check distance to each control point and return the closest one within hit radius
    if (getAttackPoint().getDistanceFrom(position) < hitRadius)
        return DragMode::Attack;
    if (getDecayPoint().getDistanceFrom(position) < hitRadius)
        return DragMode::Decay;
    if (getSustainPoint().getDistanceFrom(position) < hitRadius)
        return DragMode::Sustain;
    if (getReleasePoint().getDistanceFrom(position) < hitRadius)
        return DragMode::Release;

    return DragMode::None;
}

juce::Point<float> ADSRComponent::getAttackPoint() const
{
    auto bounds = getLocalBounds().toFloat().reduced(10);

    // Attack point: horizontal position based on attack time, vertical at peak (top)
    float x = bounds.getX() + (bounds.getWidth() * 0.25f * attackValue);
    float y = bounds.getY(); // Peak is at the top

    return {x, y};
}

juce::Point<float> ADSRComponent::getDecayPoint() const
{
    auto bounds = getLocalBounds().toFloat().reduced(10);
    auto attackPoint = getAttackPoint();

    // Decay point: positioned after attack point, vertical position based on sustain level
    float x = attackPoint.x + (bounds.getWidth() * 0.25f * decayValue);
    float y = bounds.getY() + (bounds.getHeight() * (1.0f - sustainValue));

    return {x, y};
}

juce::Point<float> ADSRComponent::getSustainPoint() const
{
    auto bounds = getLocalBounds().toFloat().reduced(10);
    auto decayPoint = getDecayPoint();

    // Sustain point: fixed horizontal position, same vertical level as decay end
    float x = bounds.getX() + bounds.getWidth() * 0.7f; // Fixed position at 70% width
    float y = decayPoint.y; // Same level as decay endpoint

    return {x, y};
}

juce::Point<float> ADSRComponent::getReleasePoint() const
{
    auto bounds = getLocalBounds().toFloat().reduced(10);
    auto sustainPoint = getSustainPoint();

    // Release point: positioned after sustain based on release time, back to zero level
    float x = sustainPoint.x + (bounds.getWidth() * 0.3f * releaseValue);
    float y = bounds.getBottom(); // Back to zero level

    return {x, y};
}

float ADSRComponent::mapToADSRValue(float screenValue, bool isTime) const
{
    // Simple linear mapping for this implementation
    // In a more advanced version, this could implement logarithmic scaling for time parameters
    return juce::jlimit(0.0f, 1.0f, screenValue);
}

float ADSRComponent::mapFromADSRValue(float adsrValue, bool isTime) const
{
    // Simple linear mapping for this implementation
    // In a more advanced version, this could implement logarithmic scaling for time parameters
    return juce::jlimit(0.0f, 1.0f, adsrValue);
}